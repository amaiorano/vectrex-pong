# Vectrex Pong

This is a simple pong clone for the Vectrex. I wrote it mainly to build a good pipeline and codebase for developing Vectrex games in C++. I started from [Joakim Larsson's VectrexTutoial](https://github.com/JoakimLarsson/VectrexTutorial), then fixed some issues with building C++, and improved the Makefile for multi-source projects. I also integrated some fixes by [Malban/PeerC](http://vectrexc.malban.de/).

# Building

The easiest way to build the game is to use the [docker image I made](https://github.com/amaiorano/gcc6809-docker) with the gcc6809 cross-compiler and dev tools:

```bash
docker pull amaiorano/gcc6809
git clone https://github.com/amaiorano/vectrex-pong.git
cd vectrex-pong
docker run -v $(pwd):/root/vectrex-pong -t -i amaiorano/gcc6809 make -C vectrex-pong
```

**Note:** On Windows (using Docker Desktop), replace ```$(pwd)``` with ```%cd%```.

