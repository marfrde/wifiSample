#ifndef _WIFISCAN_H_
#define _WIFISCAN_H_

#include <string>
#include <memory>
#include "WpaSupplicant/IWpaSupplicantDBusClient.h"
class WifiScan
{
 public:
	explicit WifiScan();

	void Entry(std::shared_ptr<nxg::DBus::WpaSupplicant::IWpaSupplicantDBusClient> wpaSupplicant);
	void Work();

 private:
	bool m_scanInProgress = false;
	bool m_scanCompleted = false;
	bool m_callbackAdded = false;
	bool m_ssidSelected = false;
	std::string m_selectedSsid = "";
	std::shared_ptr<nxg::DBus::WpaSupplicant::IWpaSupplicantDBusClient> m_wpaSupplicant = nullptr;
	std::shared_ptr<nxg::DBus::WpaSupplicant::IInterface> m_interface = nullptr;
	std::time_t m_lastScanCompleted = 0;
	std::vector<std::string > m_foundSsids;
};

#endif //_WIFISCAN_H_
