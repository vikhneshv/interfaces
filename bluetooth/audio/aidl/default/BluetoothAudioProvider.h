/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <aidl/android/hardware/bluetooth/audio/BnBluetoothAudioProvider.h>
#include <aidl/android/hardware/bluetooth/audio/LatencyMode.h>
#include <aidl/android/hardware/bluetooth/audio/SessionType.h>
#include <fmq/AidlMessageQueue.h>

using ::aidl::android::hardware::common::fmq::MQDescriptor;
using ::aidl::android::hardware::common::fmq::SynchronizedReadWrite;
using ::android::AidlMessageQueue;

using MqDataType = int8_t;
using MqDataMode = SynchronizedReadWrite;
using DataMQ = AidlMessageQueue<MqDataType, MqDataMode>;
using DataMQDesc = MQDescriptor<MqDataType, MqDataMode>;

namespace aidl {
namespace android {
namespace hardware {
namespace bluetooth {
namespace audio {

class BluetoothAudioProvider : public BnBluetoothAudioProvider {
 public:
  BluetoothAudioProvider();
  ndk::ScopedAStatus startSession(
      const std::shared_ptr<IBluetoothAudioPort>& host_if,
      const AudioConfiguration& audio_config,
      const std::vector<LatencyMode>& latency_modes,
      DataMQDesc* _aidl_return);
  ndk::ScopedAStatus endSession();
  ndk::ScopedAStatus streamStarted(BluetoothAudioStatus status);
  ndk::ScopedAStatus streamSuspended(BluetoothAudioStatus status);
  ndk::ScopedAStatus updateAudioConfiguration(
      const AudioConfiguration& audio_config);
  ndk::ScopedAStatus setLowLatencyModeAllowed(bool allowed);

  ndk::ScopedAStatus parseA2dpConfiguration(
      const CodecId& codec_id, const std::vector<uint8_t>& configuration,
      CodecParameters* codec_parameters, A2dpStatus* _aidl_return);
  ndk::ScopedAStatus getA2dpConfiguration(
      const std::vector<A2dpRemoteCapabilities>& remote_a2dp_capabilities,
      const A2dpConfigurationHint& hint,
      std::optional<audio::A2dpConfiguration>* _aidl_return);

  virtual bool isValid(const SessionType& sessionType) = 0;

 protected:
  virtual ndk::ScopedAStatus onSessionReady(DataMQDesc* _aidl_return) = 0;

  ::ndk::ScopedAIBinder_DeathRecipient death_recipient_;

  std::shared_ptr<IBluetoothAudioPort> stack_iface_;
  std::unique_ptr<AudioConfiguration> audio_config_ = nullptr;
  SessionType session_type_;
  std::vector<LatencyMode> latency_modes_;
};
}  // namespace audio
}  // namespace bluetooth
}  // namespace hardware
}  // namespace android
}  // namespace aidl
