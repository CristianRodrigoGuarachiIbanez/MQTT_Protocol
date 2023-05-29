/*
**************************************************************************************
** Project: TBM/VCM Platform
**
** Copyright (C) MAN Truck & Bus AG
**
** This document contains proprietary information belonging to MAN Truck & Bus AG.
** Passing on and copying of this document, and communication of its contents
** is not permitted without prior written authorization.
**
**************************************************************************************
*/

#ifndef UTIL_INCLUDE_UTIL_BACKENDCONFIG_H_
#define UTIL_INCLUDE_UTIL_BACKENDCONFIG_H_

#include <ecu/com/backend.h>
#include <ecu/com/backend_messageadapter.h>

#include <fstream>
#include <memory>
#include <string>

#include "Protobuf.h"

namespace util {

/**
** \brief Backend configuration class.
*/
template<class T>
class BackendConfig {
 public:
  /**
  ** \brief Class constructor.
  **
  ** \param[in] backend_client Pointer to backend client.
  ** \param[in] content_id     Content ID of configuration message.
  */
  BackendConfig(const ecu::lapi::com::BackendClient_ptr& backend_client,
                std::uint32_t content_id);

  /**
  ** \brief Class destructor.
  */
  virtual ~BackendConfig();

  /**
  ** \brief Initializes class instance.
  **
  ** \details It tries to read configuration file. If configuration file is not
  **          available or unreadable, then default configuration is sent to backend
  **          and written to configuration file upon delivery confirmation.
  **
  ** \param[in] config_path Path to persistent configuration file.
  **
  ** \return True if initialization was successful.
  */
  virtual bool Init(const std::string& config_path);

 protected:
  /**
  ** \brief Alias to configuration message type.
  */
  using Config = T;

  /**
  ** \brief Unique pointer to configuration message.
  */
  using ConfigPtr = std::unique_ptr<Config>;

  /**
  ** \brief Type definition of send callback.
  */
  using SentCallback = std::function<void()>;

  /**
  ** \brief Applies new configuration.
  **
  ** \param[in] config Configuration to be applied.
  **
  ** \return True if current configuration was updated (changes in configuration were
  **         detected).
  */
  virtual bool ApplyConfig(const Config& config) = 0;

  /**
  ** \brief Collects effective configuration parameters values.
  **
  ** \param[in] config Optional pointer to configuration received from backend.
  **
  ** \return Configuration object filled with effective values.
  */
  virtual ConfigPtr FetchConfig(const Config* config) const = 0;

  /**
  ** \brief Configuration send function.
  **
  ** \param[in] config   Configuration to be sent.
  ** \param[in] callback Function automatically called when message has been
  **                     successfully sent to backend broker.
  **
  ** \return True if configuration was successfully sent to local broker.
  */
  virtual bool SendConfig(const Config& config, const SentCallback& callback) const;

  /**
  ** \brief Writes configuration to persistent file.
  **
  ** \param[in] config Configuration to be written to file.
  **
  ** \return True if configuration was successfully written to file.
  */
  virtual bool WriteConfig(const Config& config) const;

  /**
  ** \brief Reads configuration from persistent file.
  **
  ** \return Configuration object or \c nullptr in case of error.
  */
  virtual ConfigPtr ReadConfig() const;

 private:
  ecu::lapi::com::BackendClient_ptr backend_client_;
  std::uint32_t content_id_;
  std::string config_path_;
};

template<class T>
BackendConfig<T>::BackendConfig(const ecu::lapi::com::BackendClient_ptr& backend_client,
                                std::uint32_t content_id)
    : backend_client_(backend_client),
      content_id_(content_id) {
  assert(backend_client_);
}

template<class T>
BackendConfig<T>::~BackendConfig() {
  backend_client_->clear_content_callbacks(content_id_);
}

template<class T>
bool BackendConfig<T>::Init(const std::string& config_path) {
  if (config_path.empty() || !config_path_.empty()) {
    return false;
  }

  config_path_ = config_path;

  auto config = ReadConfig();
  if (config) {
    LOG(NOTICE) << "Applying configuration from file: " << config_path_;

    if (!ApplyConfig(*config)) {
      LOG(NOTICE) << "Application is using default configuration";
    }
  } else {
    LOG(NOTICE) << "Unable to read configuration from file: " << config_path_;

    auto default_config = std::shared_ptr<Config>(FetchConfig(nullptr));
    if (default_config && !SendConfig(*default_config, [this, default_config]() {
      if (std::ifstream(config_path_)) {
        LOG(NOTICE) << "Configuration already persisted to file: " << config_path_;
      } else if (WriteConfig(*default_config)) {
        LOG(NOTICE) << "Default configuration persisted to file: " << config_path_;
      } else {
        LOG(ERROR) << "Unable to persist default configuration to file: " << config_path_;
      }
    })) {
      LOG(ERROR) << "Unable to send default configuration to backend";
    }
  }

  backend_client_->add_content_callback(content_id_, [this](auto, auto& message) {
    auto adapter = ecu::lapi::com::PbBackendMessageAdapter<Config>();
    auto result = adapter.deserialize(message);
    if (result.nok()) {
      LOG(ERROR) << "Configuration message deserialization error: " << result.err_val();
      return;
    }

    const auto& config = result.val();

    LOG(NOTICE) << "Received " << ToString(config);

    auto updated = this->ApplyConfig(config);
    auto current_config = this->FetchConfig(&config);

    if (current_config) {
      if (updated) {
        if (this->WriteConfig(*current_config)) {
          LOG(NOTICE) << "Configuration persisted to file: " << config_path_;
        } else {
          LOG(ERROR) << "Unable to persist configuration to file: " << config_path_;
        }
      }

      if (!this->SendConfig(*current_config, nullptr)) {
        LOG(ERROR) << "Unable to send current configuration to backend";
      }
    } else {
      LOG(ERROR) << "Unable to fetch current configuration";
    }
  });

  return true;
}

template<class T>
bool BackendConfig<T>::SendConfig(const Config& config,
                                  const SentCallback& callback) const {
  auto adapter = ecu::lapi::com::PbBackendMessageAdapter<Config>();
  auto result = adapter.serialize(config);
  if (result.nok()) {
    LOG(ERROR) << "Unable to serialize " << config.GetTypeName();
    return false;
  }

  LOG(NOTICE) << "Sending " << ToString(config);

  auto tx_options = ecu::lapi::com::BackendClient::TxOptions();
  tx_options.priority = 1;
  tx_options.kl15robust = false;

  if (callback) {
    tx_options.callback = [callback](auto id, auto status) {
      if (status == ecu::lapi::com::BackendClient::ResponseStatus::SENT) {
        LOG(NOTICE) << "Configuration with id " << id << " published successfully";
        callback();
      }
    };
  }

  auto id = backend_client_->publish(result.val(), content_id_, tx_options);
  if (id == 0) {
    LOG(ERROR) << "Unable to publish " << config.GetTypeName();
    return false;
  }

  LOG(NOTICE) << "Published " + config.GetTypeName() << " with id: " << id;
  return true;
}

template<class T>
bool BackendConfig<T>::WriteConfig(const Config& config) const {
  return WriteProtobufMessage(config, config_path_.c_str());
}

template<class T>
typename BackendConfig<T>::ConfigPtr BackendConfig<T>::ReadConfig() const {
  return ReadProtobufMessage<T>(config_path_.c_str());
}

}  // namespace util

#endif  // UTIL_INCLUDE_UTIL_BACKENDCONFIG_H_

/************************************************************************************/
