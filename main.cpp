#include <memory>
#include <iostream>

#include "WpaSupplicant/WpaSupplicantDBusClient.h"
#include "WifiScan.h"

int main()
{
	using namespace std::chrono_literals;

	std::shared_ptr<::DBus::StandaloneDispatcher> m_dispatcher = ::DBus::StandaloneDispatcher::create();
	std::shared_ptr<::DBus::Connection> m_connection;
	std::shared_ptr<nxg::DBus::WpaSupplicant::IWpaSupplicantDBusClient> m_wpaSupplicant = nullptr;

	// Init
	std::cout << "get dispatcher" << std::endl;
	m_connection = m_dispatcher->create_connection(::DBus::BusType::SYSTEM);

	std::cout << "get wpaSupplicant" << std::endl;
	m_wpaSupplicant = std::make_shared<nxg::DBus::WpaSupplicant::WpaSupplicantDBusClient>(m_connection);

	WifiScan scan = WifiScan();

	scan.Entry(m_wpaSupplicant);
	std::cout << "start loop" << std::endl;
	while(1)
	{
		scan.Work();
		std::this_thread::sleep_for(500ms);
	}
}
