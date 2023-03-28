RUN_TESTS_MENU_OPTION()

WINDOW_TITLE("Practice Final Exam")

MENU_ORDER("PerformanceGUI.cpp",
           "InteractiveGUI.cpp")

TEST_ORDER("AllSquaredAway.cpp",
           "AllSquaredAwayTests.cpp",
           "LeapfrogHashTable.cpp",
           "LeapfrogHashTableTests.cpp")
