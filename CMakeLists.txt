cmake_minimum_required(VERSION 3.21)

project(SlotsMachine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

qt_standard_project_setup()

qt_add_executable(SlotsMachine
    main.cpp
    MainWindow.h
    MainWindow.cpp
    src/drum.cpp
    src/symbol.cpp
    src/specialsymbol.cpp
    src/fruit.cpp
    src/lemon.cpp
    src/cherry.cpp
    src/plum.cpp
    src/bomb.cpp
    src/seven.cpp
    src/game.cpp
)

qt_add_resources(SlotsMachine "image_resources"
    PREFIX "/images"
    BASE "resources/images"
    FILES
        resources/images/machine_hq_yel.png
        resources/images/Lemon.png
        resources/images/Cherry.png
        resources/images/Plum.png
        resources/images/Bomb.png
        resources/images/Seven.png

)

qt_add_resources(SlotsMachine "sound_resources"
    PREFIX "/sounds"
    BASE "resources/sounds"
    FILES
        resources/sounds/coin_win.wav

)

qt_add_resources(SlotsMachine "video_resources"
    PREFIX "/videos"
    BASE "resources/videos"
    FILES
        resources/videos/spin.mp4      
)

target_link_libraries(SlotsMachine PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)