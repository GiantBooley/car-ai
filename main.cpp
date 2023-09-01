#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
#include <vector>
using namespace std;

bool intersects(int a, int b, int c, int d, int p, int q, int r, int s) {
  double det, gamma, lambda;
  det = (c - a) * (s - q) - (r - p) * (d - b);
  if (det == 0.0) {
    return false;
  } else {
    lambda = ((s - q) * (r - a) + (p - r) * (s - b)) / det;
    gamma = ((b - d) * (r - a) + (c - a) * (s - b)) / det;
    return (0 < lambda && lambda < 1) && (0 < gamma && gamma < 1);
  }
};
std::vector<int> intersection(int x1, int y1, int x2, int y2, int x3,int  y3, int x4, int y4) {
    double ua, ub, denom = (y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1);
    if (denom == 0) {
        return {0, 0, 0};
    }
    ua = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3))/denom;
    ub = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3))/denom;
	if (ua < 0 || ua > 1 || ub < 0 || ub > 1) {
		return {0, 0, 0};
	};
    std::vector<int> zeek = {
        x1 + (int)(ua * (x2 - x1)),
        y1 + (int)(ua * (y2 - y1)),
		1
    };
	return zeek;
}
double lerp(double a, double b, double t) {
	return (b - a) * t + a;
};
double distance(double a, double b, double c, double d) {
	return sqrt(pow(a - c, 2.0) + pow(b - d, 2.0));
};
class Track {
	public:
		std::vector<int> points[81] = {
			{21, 15},
			{116, 16},
			{222, 23},
			{268, 53},
			{283, 77},
			{307, 113},
			{322, 121},
			{353, 119},
			{416, 86},
			{470, 61},
			{521, 63},
			{565, 86},
			{592, 133},
			{598, 179},
			{598, 246},
			{576, 283},
			{558, 310},
			{568, 331},
			{574, 362},
			{564, 393},
			{541, 412},
			{505, 441},
			{464, 450},
			{422, 451},
			{391, 447},
			{372, 424},
			{355, 404},
			{336, 377},
			{317, 356},
			{296, 348},
			{271, 359},
			{243, 377},
			{239, 396},
			{219, 426},
			{164, 435},
			{118, 437},
			{88, 412},
			{69, 375},
			{57, 335},
			{54, 282},
			{62, 247},
			{51, 215},
			{34, 191},
			{30, 146},
			{23, 118},
			{22, 95},
			{29, 85},
			{54, 89},
			{74, 130},
			{97, 192},
			{107, 248},
			{112, 304},
			{125, 347},
			{155, 364},
			{201, 328},
			{254, 306},
			{318, 306},
			{353, 336},
			{388, 377},
			{426, 391},
			{478, 391},
			{527, 354},
			{513, 305},
			{536, 271},
			{550, 231},
			{553, 187},
			{524, 144},
			{511, 130},
			{465, 123},
			{423, 149},
			{388, 171},
			{316, 186},
			{292, 165},
			{257, 141},
			{244, 113},
			{190, 81},
			{146, 76},
			{105, 70},
			{44, 68},
			{15, 63},
			{27, 14}
		};
		std::vector<int> closestIntersection(int x1, int y1, int x2, int y2) {
			std::vector<int> closestIntersectionPoint = {0, 0, 0};
			double closestIntersectionDistance = 0;
			for (int i = 0; i < 80; i++) {
				std::vector<int> it = intersection(x1, y1, x2, y2, points[i][0], points[i][1], points[i + 1][0], points[i + 1][1]);
				double intersectionDistance = distance(x1, y1, it[0], it[1]);
				if ((closestIntersectionPoint[2] == 0 || intersectionDistance < closestIntersectionDistance) && it[2] == 1) {
					closestIntersectionPoint = it;
					closestIntersectionDistance = intersectionDistance;
				};
			};
			return closestIntersectionPoint;
		};
};
class Car {
	public:
		double x = 45;
		double y = 45;
		double dir = 3.141592653589 / 2.0;
		double vel = 0;
		std::vector<int> rays[8];
		void reset() {
			x = 45;
			y = 45;
			dir = 3.141592653589 / 2.0;
			vel = 0;
		};
		void raycast(double fov, int rayLength, unsigned int numberOfRays, Track track) {
			for (unsigned int i = 0; i < numberOfRays; i++) {
				double rayDir = dir + lerp(-fov / 2.0, fov / 2.0, (double)i / (double)numberOfRays);
				std::vector<int> rayEndPoint = {
					(int)(x + sin(rayDir) * (double)rayLength),
					(int)(y + cos(rayDir) * (double)rayLength)
				};
				std::vector<int> rayIntersectionPoint = track.closestIntersection(x, y, rayEndPoint[0], rayEndPoint[1]);
				rays[(int)i] = (rayIntersectionPoint[2] == 1) ? (std::vector<int>){rayIntersectionPoint[0], rayIntersectionPoint[1]} : rayEndPoint;//(int)distance(x, y, rayEndPoint[0], rayEndPoint[1]);
			};
		};
};
void render(SDL_Renderer* renderer, Car (&cars)[2], Track track) {
	SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
	SDL_RenderClear(renderer);
	for (Car car : cars) {
		SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
		for (unsigned int i = 0; i < 80; i++) {
			SDL_RenderDrawLine(renderer, track.points[i][0], track.points[i][1], track.points[i + 1][0], track.points[i + 1][1]);
		};

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		double pi = 3.141592653589;
		double carSize = 20;
		std::vector<int> carPoints[4] = {
			{(int)(carSize * sin(car.dir - pi / 4)),     (int)(carSize * cos(car.dir - pi / 4))},
			{(int)(carSize * sin(car.dir + pi / 4)),     (int)(carSize * cos(car.dir + pi / 4))},
			{(int)(carSize * sin(car.dir + pi * 3 / 4)), (int)(carSize * cos(car.dir + pi * 3 / 4))},
			{(int)(carSize * sin(car.dir - pi * 3 / 4)), (int)(carSize * cos(car.dir - pi * 3 / 4))}
		};
		SDL_RenderDrawLine(renderer, carPoints[0][0] + car.x, carPoints[0][1] + car.y, carPoints[1][0] + car.x, carPoints[1][1] + car.y);
		SDL_RenderDrawLine(renderer, carPoints[1][0] + car.x, carPoints[1][1] + car.y, carPoints[2][0] + car.x, carPoints[2][1] + car.y);
		SDL_RenderDrawLine(renderer, carPoints[2][0] + car.x, carPoints[2][1] + car.y, carPoints[3][0] + car.x, carPoints[3][1] + car.y);
		SDL_RenderDrawLine(renderer, carPoints[3][0] + car.x, carPoints[3][1] + car.y, carPoints[0][0] + car.x, carPoints[0][1] + car.y);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, car.x, car.y, car.x + sin(car.dir) * 50, car.y + cos(car.dir) * 50);

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		for (int i = 0; i < 8; i++) {
			SDL_RenderDrawLine(renderer, car.x, car.y, car.rays[i][0], car.rays[i][1]);
		};
	};

		SDL_RenderPresent(renderer);
};
void tickCar(Car &car, Track track) {
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	car.vel *= 0.95;
	car.vel += (state[SDL_SCANCODE_UP] - state[SDL_SCANCODE_DOWN]) * 0.4;
	car.dir -= (state[SDL_SCANCODE_RIGHT] - state[SDL_SCANCODE_LEFT]) * 0.1;
    car.x += sin(car.dir) * car.vel;
	car.y += cos(car.dir) * car.vel;
	double pi = 3.141592653589;
	double carSize = 20;
	std::vector<int> carPoints[4] = {
		{(int)(carSize * sin(car.dir - pi / 4)),     (int)(carSize * cos(car.dir - pi / 4))},
		{(int)(carSize * sin(car.dir + pi / 4)),     (int)(carSize * cos(car.dir + pi / 4))},
		{(int)(carSize * sin(car.dir + pi * 3 / 4)), (int)(carSize * cos(car.dir + pi * 3 / 4))},
		{(int)(carSize * sin(car.dir - pi * 3 / 4)), (int)(carSize * cos(car.dir - pi * 3 / 4))}
	};
	for (int i = 0; i < 80; i++) {
		if (
			intersects(car.x + carPoints[0][0], car.y + carPoints[0][1], car.x + carPoints[1][0], car.y + carPoints[1][1], track.points[i][0], track.points[i][1], track.points[i + 1][0], track.points[i + 1][1]) ||
			intersects(car.x + carPoints[1][0], car.y + carPoints[1][1], car.x + carPoints[2][0], car.y + carPoints[2][1], track.points[i][0], track.points[i][1], track.points[i + 1][0], track.points[i + 1][1]) ||
			intersects(car.x + carPoints[2][0], car.y + carPoints[2][1], car.x + carPoints[3][0], car.y + carPoints[3][1], track.points[i][0], track.points[i][1], track.points[i + 1][0], track.points[i + 1][1]) ||
			intersects(car.x + carPoints[3][0], car.y + carPoints[3][1], car.x + carPoints[0][0], car.y + carPoints[0][1], track.points[i][0], track.points[i][1], track.points[i + 1][0], track.points[i + 1][1])
		) {
			car.reset();
			break;
		};
	};
	car.raycast(3.0, 200, 8, track);
};
void wait(int ms) {
	clock_t endwait;
	endwait = clock() + ms;
	while (clock() < endwait) {};
};
int main(int argc, char *argv[]) {
	SDL_Window* window = SDL_CreateWindow("car ai", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	Car cars[2];
	cars[1].x += 20;
	Track track;
	bool keep_window_open = true;
	while(keep_window_open) {
		SDL_Event e;
		while(SDL_PollEvent(&e) > 0) {
			switch(e.type) {
				case SDL_QUIT:
					keep_window_open = false;
					break;
			};
			SDL_UpdateWindowSurface(window);
		};
		tickCar(cars[0], track);
		tickCar(cars[1], track);
		render(renderer, cars, track);
		wait((int)floor(1000 / 60));
	};
	return 0;
};
