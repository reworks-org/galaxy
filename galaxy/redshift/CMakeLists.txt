project(redshift CXX)

set(
redshift_src
"src/redshift/dummy.cpp"
)

source_group(TREE "" FILES ${redshift_src})
add_library("redshift" STATIC ${redshift_src})
