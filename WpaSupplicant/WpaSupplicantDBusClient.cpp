#include <iostream>
#include "WpaSupplicantDBusClient.h"
#include "IWpaSupplicantDBusClient.h"
#include <dbus-cxx-2.0/dbus-cxx/object.h>
#include <dbus-cxx-2.0/dbus-cxx/variant.h>


namespace nxg::DBus
{
	constexpr auto WpaSupplicantDestination = "fi.w1.wpa_supplicant1";
	constexpr auto WpaSupplicantPath = "/fi/w1/wpa_supplicant1";

	constexpr auto WpaSupplicantInterface = "fi.w1.wpa_supplicant1";
	constexpr auto WpaSupplicantInterfaceInterface = "fi.w1.wpa_supplicant1.Interface";
	constexpr auto WpaSupplicantBssInterface = "fi.w1.wpa_supplicant1.BSS";
	constexpr auto WpaSupplicantNetworkInterface = "fi.w1.wpa_supplicant1.Network";


	class WpaSupplicantObject : public ::DBus::ObjectProxy
	{
	protected:
		explicit WpaSupplicantObject(std::shared_ptr<::DBus::Connection> connection)
			: ::DBus::ObjectProxy(std::move(connection), WpaSupplicantDestination, WpaSupplicantPath)
		{
			m_methodGetInterface = this->create_method<::DBus::Path(std::string)>(WpaSupplicantInterface, "GetInterface");
		}

	public:
		static std::shared_ptr<WpaSupplicantObject> Create(const std::shared_ptr<::DBus::Connection>& connection)
		{
			auto wpaSupplicantObject = std::shared_ptr<WpaSupplicantObject>(new WpaSupplicantObject(connection));
			connection->register_object_proxy(wpaSupplicantObject);
			return wpaSupplicantObject;
		}

		::DBus::Path GetInterface(const std::string& interfaceName)
		{
			return (*m_methodGetInterface)(interfaceName);
		}


	protected:
		std::shared_ptr<::DBus::MethodProxy<::DBus::Path(std::string)>> m_methodGetInterface;
	};

	class WpaSupplicantInterfaceObject : public ::DBus::ObjectProxy
	{
	protected:
		explicit WpaSupplicantInterfaceObject(std::shared_ptr<::DBus::Connection> connection, const ::DBus::Path& interfacePath)
			: ::DBus::ObjectProxy(std::move(connection), WpaSupplicantDestination, interfacePath)
		{
			m_methodScan = this->create_method<void(std::map<std::string, ::DBus::Variant>)>(WpaSupplicantInterfaceInterface, "Scan");

			m_signalScanDone = this->create_signal<void(bool)>(WpaSupplicantInterfaceInterface, "ScanDone");
			m_propertyBSSs = this->create_property<std::vector<::DBus::Path>>(WpaSupplicantInterfaceInterface, "BSSs");
			m_propertyNetworks = this->create_property<std::vector<::DBus::Path>>(WpaSupplicantInterfaceInterface, "Networks");
		}

	public:
		static std::shared_ptr<WpaSupplicantInterfaceObject> Create(const std::shared_ptr<::DBus::Connection>& connection, const ::DBus::Path& interfacePath)
		{
			auto wpaSupplicantInterfaceObject = std::shared_ptr<WpaSupplicantInterfaceObject>(new WpaSupplicantInterfaceObject(connection, interfacePath));
			connection->register_object_proxy(wpaSupplicantInterfaceObject);
			return wpaSupplicantInterfaceObject;
		}

		void Scan(const std::string& scanType)
		{
			std::map<std::string, ::DBus::Variant> args = {
					{"Type", scanType},
			};

			return (*m_methodScan)(args);
		}

		void AddScanDoneCallback(const std::function<void(bool)>& callback)
		{
			m_signalScanDone->connect(callback);
		}

		std::vector<::DBus::Path> GetBSSs()
		{
			return m_propertyBSSs->value();
		}
		std::vector<::DBus::Path> GetNetworks()
		{
			return m_propertyNetworks->value();
		}

	private:
		std::shared_ptr<::DBus::MethodProxy<void(std::map<std::string, ::DBus::Variant>)>> m_methodScan;

		std::shared_ptr<::DBus::SignalProxy<void(bool)>> m_signalScanDone;
		std::shared_ptr<::DBus::PropertyProxy<std::vector<::DBus::Path>>> m_propertyBSSs;
		std::shared_ptr<::DBus::PropertyProxy<std::vector<::DBus::Path>>> m_propertyNetworks;
	};

	class WpaSupplicantBssObject : public ::DBus::ObjectProxy
	{
	protected:
		explicit WpaSupplicantBssObject(std::shared_ptr<::DBus::Connection> connection, const ::DBus::Path& interfacePath)
			: ::DBus::ObjectProxy(std::move(connection), WpaSupplicantDestination, interfacePath)
		{
			m_propertySSID = this->create_property<std::vector<uint8_t>>(WpaSupplicantBssInterface, "SSID");
			m_propertySignal = this->create_property<int16_t>(WpaSupplicantBssInterface, "Signal");
		}

	public:
		static std::shared_ptr<WpaSupplicantBssObject> Create(const std::shared_ptr<::DBus::Connection>& connection, const ::DBus::Path& interfacePath)
		{
			auto wpaSupplicantBssObject = std::shared_ptr<WpaSupplicantBssObject>(new WpaSupplicantBssObject(connection, interfacePath));
			connection->register_object_proxy(wpaSupplicantBssObject);
			return wpaSupplicantBssObject;
		}

		std::vector<uint8_t> GetSSID()
		{
			return m_propertySSID->value();
		}

		int16_t GetSignal()
		{
			return m_propertySignal->value();
		}

	private:
		std::shared_ptr<::DBus::PropertyProxy<std::vector<uint8_t>>> m_propertySSID;
		std::shared_ptr<::DBus::PropertyProxy<int16_t>> m_propertySignal;
	};

	class WpaSupplicantNetworkObject : public ::DBus::ObjectProxy
	{
	protected:
		explicit WpaSupplicantNetworkObject(std::shared_ptr<::DBus::Connection> connection, const ::DBus::Path& interfacePath)
			: ::DBus::ObjectProxy(std::move(connection), WpaSupplicantDestination, interfacePath)
		{
			m_propertyEnabled = this->create_property<bool>(WpaSupplicantNetworkInterface, "Enabled");
			m_propertyProperties = this->create_property<std::map<std::string, ::DBus::Variant>>(WpaSupplicantNetworkInterface, "Properties");
		}

	public:
		static std::shared_ptr<WpaSupplicantNetworkObject> Create(const std::shared_ptr<::DBus::Connection>& connection, const ::DBus::Path& interfacePath)
		{
			auto wpaSupplicantNetworkObject = std::shared_ptr<WpaSupplicantNetworkObject>(new WpaSupplicantNetworkObject(connection, interfacePath));
			connection->register_object_proxy(wpaSupplicantNetworkObject);
			return wpaSupplicantNetworkObject;
		}

		bool GetEnabled()
		{
			return m_propertyEnabled->value();
		}

		std::map<std::string, ::DBus::Variant> GetProperties()
		{
			return m_propertyProperties->value();
		}

	private:
		std::shared_ptr<::DBus::PropertyProxy<bool>> m_propertyEnabled;
		std::shared_ptr<::DBus::PropertyProxy<std::map<std::string, ::DBus::Variant>>> m_propertyProperties;
	};


	namespace WpaSupplicant
	{
		BSS::BSS(const std::shared_ptr<::DBus::Connection>& connection, const ::DBus::Path& bssPath)
		{
			m_object = WpaSupplicantBssObject::Create(connection, bssPath);
		}

		std::string BSS::GetSSID()
		{
			auto ssid = m_object->GetSSID();
			/*std::cout << "got ssid vector of size: " << ssid.size() << " ssid characters: ";

			std::for_each(ssid.begin(), ssid.end(),[](unsigned char& character)  {
				std::cout << character;
			});

			std::cout << std::endl;*/
			return std::string{ssid.begin(), ssid.end()};
		}

		int16_t BSS::GetSignalStrength()
		{
			return m_object->GetSignal();
		}

		Interface::Interface(std::shared_ptr<::DBus::Connection> connection, const ::DBus::Path& interfacePath)
			: m_connection(std::move(connection))
		{
			m_object = WpaSupplicantInterfaceObject::Create(m_connection, interfacePath);
		}

		void Interface::StartScan()
		{
			m_object->Scan("active");
		}

		void Interface::AddScanDoneCallback(const std::function<void(bool)>& callback)
		{
			m_object->AddScanDoneCallback(callback);
		}

		std::vector<std::shared_ptr<IBSS>> Interface::GetScanResult()
		{
			std::vector<std::shared_ptr<IBSS>> result;

			try
			{
				auto bssPaths = m_object->GetBSSs();
				if (bssPaths.empty())
				{
					std::cout << "Received BSSs are empty" << std::endl;
					return {};
				}

				for (const auto& bssPath : bssPaths)
				{
					result.push_back(std::make_shared<BSS>(m_connection, bssPath));
				}

			}
			catch (std::exception const& e)
			{
				std::cout << "BSS receiving failed with exception " << e.what() << std::endl;
				std::cerr << "BSS receiving failed with exception " << e.what() << std::endl;
			}
			return result;
		}

		WpaSupplicantDBusClient::WpaSupplicantDBusClient()
		{
			try
			{
				m_dispatcher = ::DBus::StandaloneDispatcher::create();
				m_connection = m_dispatcher->create_connection(::DBus::BusType::SYSTEM);

				initDBusObjects();
			}
			catch (const std::shared_ptr<::DBus::Error>& ex)
			{
				std::cout << "Init Dbus failed: " <<  ex->message() << std::endl;
			}
			catch (...)
			{
				std::exception_ptr p = std::current_exception();
				std::string ex = (p ? p.__cxa_exception_type()->name() : "null");
				std::cout << "Call initDbus failed: " << ex << std::endl;
			}
		}

		WpaSupplicantDBusClient::WpaSupplicantDBusClient(std::shared_ptr<::DBus::Connection> connection)
			: m_connection(std::move(connection))
		{
			initDBusObjects();
		}


		std::shared_ptr<IInterface> WpaSupplicantDBusClient::GetInterface(const std::string& interfaceName)
		{
			auto interfacePath = m_wpaSupplicantObject->GetInterface(interfaceName);
			if (interfacePath.empty())
			{
				std::cout << "Interface path is empty" << std::endl;
				return nullptr;
			}

			std::cout << "Got path '" << interfacePath << "' for interface '" << interfaceName << "'" << std::endl;

			return std::make_shared<WpaSupplicant::Interface>(m_connection, interfacePath);
		}


		void WpaSupplicantDBusClient::initDBusObjects()
		{
			if (!m_connection)
			{
				std::cout << "Connection not initialized" << std::endl;
				return;
			}

			try
			{
				m_wpaSupplicantObject = WpaSupplicantObject::Create(m_connection);
			}
			catch (const std::shared_ptr<::DBus::Error>& ex)
			{
				std::cout << "Caught Dbus exception: " << ex->message() << std::endl;
			}
			catch (...)
			{
				std::exception_ptr p = std::current_exception();
				std::string ex = (p ? p.__cxa_exception_type()->name() : "null");
				std::cout << "Caught exception: " << ex << std::endl;
			}
		}
	}// namespace WpaSupplicant
}// namespace nxg::DBus
