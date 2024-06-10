#include <ctime>
#include <iostream>
#include "WifiScan.h"


static const int c_wifiScanInterval = 15;
static const char* const c_WifiInterfaceName = "wlan0";

WifiScan::WifiScan()
{

}

void WifiScan::Entry(std::shared_ptr<nxg::DBus::WpaSupplicant::IWpaSupplicantDBusClient> wpaSupplicant)
{
	std::cout << "Entry" << std::endl;

	if(m_wpaSupplicant == nullptr)
	{
		m_wpaSupplicant = wpaSupplicant;
		m_interface = m_wpaSupplicant->GetInterface(c_WifiInterfaceName);

		m_interface->AddScanDoneCallback([this](bool var) {
			std::cout << "callback: network scan done" << std::endl;
			this->m_scanInProgress = false;
			this->m_scanCompleted = true;
			auto result = this->m_interface->GetScanResult();

			this->m_foundSsids = std::vector<std::string>();

			for (int i = 0; i < result.size(); i++)
			{
				auto ssid = result[i]->GetSSID();
				std::cout << "found " << ssid << std::endl;
				this->m_foundSsids.push_back(ssid);
			}
			time(&this->m_lastScanCompleted);
		});
	}
}

void WifiScan::Work()
{
	if (m_scanInProgress && !m_scanCompleted)
	{
		return;
	}
	if (!m_scanInProgress && !m_scanCompleted)
	{
		std::time_t now;
		time(&now);
		auto elapsed = difftime(now, m_lastScanCompleted);
		if (elapsed > c_wifiScanInterval)
		{

			std::cout << "network scan started" << std::endl;
			m_scanInProgress = true;
			m_interface->StartScan();
		}
	}
	if (m_scanCompleted)
	{
		std::cout << "process Scan result" << std::endl;
		std::cout << "got " << this->m_foundSsids.size() << " results" << std::endl;

		// real project will use the ssid strings at this location.

		m_scanCompleted = false;
	}
}