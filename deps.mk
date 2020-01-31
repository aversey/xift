attribute.o: src/attribute.cpp src/attribute.hpp src/utils.hpp
exporter.o: src/exporter.cpp src/exporter.hpp
file.o: src/file.cpp src/file.hpp src/exporter.hpp
str.o: src/str.cpp src/str.hpp src/exporter.hpp src/utils.hpp
tag.o: src/tag.cpp src/tag.hpp src/attribute.hpp src/utils.hpp
utils.o: src/utils.cpp src/utils.hpp
xift.o: src/xift.cpp src/xift.hpp src/exporter.hpp src/tag.hpp \
 src/attribute.hpp src/utils.hpp
