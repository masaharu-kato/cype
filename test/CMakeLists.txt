# CMakeList.txt : cype の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
cmake_minimum_required (VERSION 3.8)

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (typed_set_test "typed_set_test.cpp")
add_executable (array_test "array_test.cpp")
add_executable (md_array_test "md_array_test.cpp")
add_executable (typed_array_test "typed_array_test.cpp")
add_executable (typed_vector_test "typed_vector_test.cpp")
add_executable (tmplval_list_test "tmplval_list_test.cpp")


# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
