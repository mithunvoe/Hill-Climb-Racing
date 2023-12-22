name = main
#OBJS specifies which files to compile as part of the project
OBJS = $(name).cpp
#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = $(name)

LINKER_OBJ = TextureManager.cpp Game.cpp GameObject.cpp Vector2D.cpp Collision.cpp highscore.cpp ECS/ECS.cpp ECS/Component.hpp 
#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = 

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer


#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(LINKER_OBJ) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME); ./$(OBJ_NAME)