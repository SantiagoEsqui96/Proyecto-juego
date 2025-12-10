# Directorios de origen y destino
SRC_DIR := src
BIN_DIR := bin

SFML := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d

# Obtener todos los archivos .cpp en el directorio de origen, excepto Tablero.cpp y Enemigo.cpp
CPP_FILES := $(filter-out $(SRC_DIR)/Tablero.cpp $(SRC_DIR)/Enemigo.cpp,$(wildcard $(SRC_DIR)/*.cpp))

# Generar los nombres de los archivos .exe en el directorio de destino
EXE_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.exe,$(CPP_FILES))

# Regla especial para 00_Ventana que necesita Tablero.cpp, Enemigo.cpp, GameState.cpp y AudioManager.cpp
$(BIN_DIR)/00_Ventana.exe: $(SRC_DIR)/00_Ventana.cpp $(SRC_DIR)/Tablero.cpp $(SRC_DIR)/Enemigo.cpp $(SRC_DIR)/GameState.cpp $(SRC_DIR)/AudioManager.cpp
	g++ $^ -o $@ $(SFML) -Iinclude

# Regla para compilar cada archivo .cpp y generar el archivo .exe correspondiente
$(BIN_DIR)/%.exe: $(SRC_DIR)/%.cpp
	g++ $< -o $@ $(SFML) -Iinclude

# Regla por defecto para compilar todos los archivos .cpp
all: $(EXE_FILES)

# Regla para ejecutar cada archivo .exe
run%: $(BIN_DIR)/%.exe
	./$<

# Regla para limpiar los archivos generados
clean:
	rm -f $(EXE_FILES)

.PHONY: all clean
.PHONY: run-%
