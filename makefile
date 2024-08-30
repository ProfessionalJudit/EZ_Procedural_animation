compile:
	g++ *.cpp -Iinclude  -Llib -lsfml-graphics -lsfml-window -lsfml-system  
compile-s:
	g++ *.cpp -Iinclude -Llib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lX11 -lopengl32 -DSFML_STATIC