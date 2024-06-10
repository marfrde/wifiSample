#ifndef WPASUPPLICANTDBUSCLIENT_H
#define WPASUPPLICANTDBUSCLIENT_H

#include "IWpaSupplicantDBusClient.h"

namespace nxg::DBus
{
	class WpaSupplicantObject;
	class WpaSupplicantInterfaceObject;
	class WpaSupplicantBssObject;
	class WpaSupplicantNetworkObject;

	namespace WpaSupplicant
	{

		class BSS : public IBSS
		{
		public:
			BSS(const std::shared_ptr<::DBus::Connection>& connection, const ::DBus::Path& bssPath);

			std::string GetSSID();
			int16_t GetSignalStrength();

		private:
			std::shared_ptr<WpaSupplicantBssObject> m_object;
		};


		class Interface : public IInterface
		{
		public:
			Interface(std::shared_ptr<::DBus::Connection> connection, const ::DBus::Path& interfacePath);

			void StartScan() override;
			void AddScanDoneCallback(const std::function<void(bool)>& callback) override;

			std::vector<std::shared_ptr<IBSS>> GetScanResult() override;

		private:
			std::shared_ptr<::DBus::Connection> m_connection;

			std::shared_ptr<WpaSupplicantInterfaceObject> m_object;
		};

		class WpaSupplicantDBusClient : public nxg::DBus::WpaSupplicant::IWpaSupplicantDBusClient
		{
		public:
			WpaSupplicantDBusClient();
			explicit WpaSupplicantDBusClient(std::shared_ptr<::DBus::Connection> connection);

			std::shared_ptr<IInterface> GetInterface(const std::string& interfaceName) override;

		private:
			void initDBusObjects();

		private:
			std::shared_ptr<::DBus::StandaloneDispatcher> m_dispatcher;
			std::shared_ptr<::DBus::Connection> m_connection;

			// Dbus objects
			std::shared_ptr<WpaSupplicantObject> m_wpaSupplicantObject;
		};
	}// namespace WpaSupplicant
}// namespace nxg

#endif//WPASUPPLICANTDBUSCLIENT_H
