file(REMOVE_RECURSE
  "we/CallDialog.qml"
  "we/NetizenChatUI.qml"
  "we/NotificationPanel.qml"
  "we/Toast.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/appwe_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
