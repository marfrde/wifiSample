#ifndef IWPASUPPLICANTDBUSCLIENT_H
#define IWPASUPPLICANTDBUSCLIENT_H

#include <dbus-cxx-2.0/dbus-cxx.h>
#include <dbus-cxx-2.0/dbus-cxx/connection.h>
#include <dbus-cxx-2.0/dbus-cxx/standalonedispatcher.h>

#include <optional>

namespace nxg::DBus
{
	class WpaSupplicantObject;
	class WpaSupplicantInterfaceObject;
	class WpaSupplicantBssObject;
	class WpaSupplicantNetworkObject;

	namespace WpaSupplicant
	{
		class IBSS
		{
		public:
			virtual std::string GetSSID() = 0;
			virtual int16_t GetSignalStrength() = 0;
		};

		class IInterface
		{
		public:
			virtual void StartScan() = 0;
			virtual void AddScanDoneCallback(const std::function<void(bool)>& callback) = 0;

			virtual std::vector<std::shared_ptr<IBSS>> GetScanResult() = 0;
		};

		class IWpaSupplicantDBusClient
		{
		public:
			virtual ~IWpaSupplicantDBusClient() = default;
			virtual std::shared_ptr<IInterface> GetInterface(const std::string& interfaceName) = 0;
		};
	}// namespace WpaSupplicant
}// namespace nxg

#endif//IWPASUPPLICANTDBUSCLIENT_H
