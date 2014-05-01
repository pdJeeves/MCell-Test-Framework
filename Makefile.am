AUTOMAKE_OPTIONS = foreign
man_MANS = man/TestMCell

INCLUDES = -I$(srcdir)/include

bin_PROGRAMS = TestMCell
TestMCell_SOURCES = src/average.cpp src/b_string.cpp src/config.cpp src/data_column.cpp src/data_table.cpp src/double_table_reader.cpp src/file_lexer.cpp src/main.cpp src/MCellInstance.cpp src/MCell_test.cpp src/pcl.c src/pcl_private.c src/pcl_version.c src/python_interface.cpp src/query_parser.cpp src/scanner_base.cpp src/script_file_parser.cpp src/script_manager.cpp src/table_lexer.cpp src/least_edit_distance.cpp
AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(LDFLAGS_BOOST) \
    $(LDFLAGS_BOOST_LIBS)
LIBS += $(LIBS_BOOST)

ACLOCAL_AMFLAGS = -I  m4

LDFLAGS_BOOST = \
    $(BOOST_SYSTEM_LDFLAGS) \
    $(BOOST_PROGRAM_OPTIONS_LDFLAGS)

LIBS_BOOST = \
    -lboost_system$(BOOST_LIB_SUFFIX) \
	-lboost_filesystem$(BOOST_LIB_SUFFIX) \
    -lboost_python$(BOOST_LIB_SUFFIX) \
	`python-config --ldflags`

AM_CPPFLAGS += $(BOOST_CPPFLAGS)
AM_LDFLAGS += $(LDFLAGS_BOOST)
LIBS += $(LIBS_BOOST)

