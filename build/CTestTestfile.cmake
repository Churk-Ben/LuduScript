# CMake generated Testfile for 
# Source directory: F:/Code Projects/LuduScript
# Build directory: F:/Code Projects/LuduScript/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_poker_generation "F:/Code Projects/LuduScript/build/bin/luduscript.exe" "F:/Code Projects/LuduScript/scripts/generate_poker_deck.script" "--output" "F:/Code Projects/LuduScript/output/test_poker.json")
set_tests_properties(test_poker_generation PROPERTIES  WORKING_DIRECTORY "F:/Code Projects/LuduScript/build/bin" _BACKTRACE_TRIPLES "F:/Code Projects/LuduScript/CMakeLists.txt;98;add_test;F:/Code Projects/LuduScript/CMakeLists.txt;0;")
add_test(test_werewolf_generation "F:/Code Projects/LuduScript/build/bin/luduscript.exe" "F:/Code Projects/LuduScript/scripts/generate_werewolf_cards.script" "--output" "F:/Code Projects/LuduScript/output/test_werewolf.json")
set_tests_properties(test_werewolf_generation PROPERTIES  WORKING_DIRECTORY "F:/Code Projects/LuduScript/build/bin" _BACKTRACE_TRIPLES "F:/Code Projects/LuduScript/CMakeLists.txt;103;add_test;F:/Code Projects/LuduScript/CMakeLists.txt;0;")
