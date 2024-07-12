cmake_minimum_required(VERSION 3.28)
project(Dita)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CORE
        core/analyzer/ssh_analyzer.cpp
        core/analyzer/tcp_analyzer.cpp

        #Utils
        core/analyzer/utils/ip_utils.cpp
        core/analyzer/utils/map_util.cpp
        core/analyzer/icmp_analyzer.cpp
        core/analyzer/Analyzer.cpp
        core/analyzer/Analyzer.h
        core/analyzer/utils/Utils.cpp
        core/analyzer/utils/Utils.h
)
set(UI
        Graph/Graph.cpp
        Graph/Graph.h
        DevHandler.cpp
        DevHandler.h
        ui/line_graph.cpp
        ui/service_tab.cpp
        ui/stats_view.cpp
        ui/netutil_tab.cpp
        ui/netutil_tab.h
        ui/alerts.cpp
)

set (NETHOGS
        core/nethogs/src/connection.cpp
        core/nethogs/src/conninode.cpp
        core/nethogs/src/cui.cpp
        core/nethogs/src/decpcap.cpp
        core/nethogs/src/devices.cpp
        core/nethogs/src/inode2prog.cpp
        core/nethogs/src/nethogs.h
        core/nethogs/src/packet.cpp
        core/nethogs/src/process.cpp
        ui/line.cpp
        ui/line.h
        core/analyzer/udp_analyzer.cpp

)

find_package(Qt6 COMPONENTS REQUIRED
        Core
        Gui
        Widgets
        PrintSupport
        Charts
)
find_package(PcapPlusPlus CONFIG REQUIRED)

add_executable(Dita main.cpp ${CORE} ${UI} ${NETHOGS}
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui)

target_link_libraries(Dita
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::PrintSupport
        Qt::Charts
        #PcapPlusPlus Links
        PCAP::PCAP
        PcapPlusPlus::Pcap++
        PcapPlusPlus::Common++
        PcapPlusPlus::Packet++
        ncurses
)

target_include_directories(Dita PRIVATE core/nethogs/src)
