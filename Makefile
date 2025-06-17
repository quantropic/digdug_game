CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SFML_AUDIO_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
TARGETS = digdug digdug_sfml digdug_sfml_sprites digdug_sfml_sprites_music

all: $(TARGETS)

digdug: digdug.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

digdug_sfml: digdug_sfml.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(SFML_LIBS)

digdug_sfml_sprites: digdug_sfml_sprites.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(SFML_LIBS)

digdug_sfml_sprites_music: digdug_sfml_sprites_music.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(SFML_AUDIO_LIBS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean
