CC := g++
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
EXECUTABLE := $(BIN_DIR)/program

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

CPPFLAGS := -Isrc -I/usr/include/opencascade --std=c++17
CFLAGS := -Wall -Wno-narrowing -g `pkg-config --cflags glfw3`
LDFLAGS := `pkg-config --libs glfw3` -lTKernel -lTKMath -lTKG2d -lTKG3d -lTKGeomBase -lTKBRep -lTKGeomAlgo -lTKTopAlgo -lTKPrim -lTKShHealing -lTKBO -lTKBool -lTKHLR -lTKFillet -lTKFeat -lTKOffset -lTKMesh -lTKXMesh -lTKService -lTKV3d -lTKOpenGl -lTKMeshVS -lTKCDF -lTKLCAF -lTKCAF -lTKBinL -lTKXmlL -lTKBin -lTKXml -lTKStdL -lTKStd -lTKXSBase -lTKSTEPBase -lTKSTEPAttr -lTKSTEP209 -lTKSTEP -lTKIGES -lTKXCAF -lTKXDEIGES -lTKXmlXCAF -lTKBinXCAF -lTKXDESTEP -lTKDraw -lTKTopTest -lTKViewerTest -lTKXSDRAW -lTKDCAF -lTKXDEDRAW -lTKQADraw -lTKTObjDRAW
LDLIBS :=

all: $(EXECUTABLE)
.PHONY: all

$(EXECUTABLE): $(OBJ) $(IMGUI_OBJ) $(PROTOBUF_OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(EXECUTABLE) $(OBJ_DIR)

