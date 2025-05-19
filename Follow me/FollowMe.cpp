#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include<iostream>
//



template <class TL , class TR>
constexpr auto circ_add
(const TL& lhs,
	const TR& rhs,
	const decltype(lhs + rhs)rmin = 0,
	const decltype(lhs + rhs)rmax = 360)
{

	auto c = lhs + rhs;
	auto range = rmax - rmin;
	while (c >= rmax)c -= range;
	while (c < rmin)c += range;
	return c; 
}


template <class TL, class TR>
constexpr auto circ_sub (const TL& lhs,
	const TR& rhs,
	const decltype(lhs - rhs)rmin = 0,
	const decltype(lhs + rhs)rmax = 360) {


	auto c = lhs - rhs;
	auto range = rmax - rmin;
	while (c >= rmax)c -= range;
	while (c < rmin)c += range;
	return c;

}

template <class TL, class TR>
constexpr auto circ_mul(const TL& lhs, const TR& rhs, const decltype(lhs* rhs) rmin = 0, const decltype(lhs* rhs) rmax = 360)
{
	auto c = lhs * rhs;
	auto range = rmax - rmin;
	while (c >= rmax)  c -= range;
	while (c < rmin)  c += range;
	return c;
}

template<class TL, class TR>
constexpr auto circ_div(const TL& lhs, const TR& rhs, const decltype(lhs / rhs) rmin = 0, const decltype(lhs / rhs) rmax = 360)
{
	auto c = lhs / rhs;
	auto range = rmax - rmin;
	while (c >= rmax)  c -= range;
	while (c < rmin)  c += range;
	return c;
}

template<class TL, class TR>
constexpr auto circ_lerp(const double t, const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360)
{
	return circ_add(lhs, circ_sub(rhs, lhs, rmin, rmax) * t, rmin, rmax);
}
template<class TL, class TR>
constexpr auto circ_shortdist(const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360)
{
	auto a = circ_sub(rhs, lhs, rmin, rmax);
	auto b = circ_sub(lhs, rhs, rmin, rmax);
	return std::min(a, b);
}

template<class TL, class TR>
constexpr auto circ_longdist(const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360)
{
	auto a = circ_sub(rhs, lhs, rmin, rmax);
	auto b = circ_sub(lhs, rhs, rmin, rmax);
	return std::max(a, b);
}


template<class TL, class TR>
constexpr auto circ_shortdiff(const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360)
{
	auto a = circ_sub(rhs, lhs, rmin, rmax);
	auto b = circ_sub(lhs, rhs, rmin, rmax);
	if (b > a)
		return a;
	else
		return -b;
}

template<class TL, class TR>
constexpr auto circ_shortlerp(const double t, const TL& lhs, const TR& rhs, const decltype(lhs - rhs) rmin = 0, const decltype(lhs - rhs) rmax = 360)
{
	auto a = circ_sub(rhs, lhs, rmin, rmax);
	auto b = circ_sub(lhs, rhs, rmin, rmax);
	if (a < b)
		return circ_add(lhs, a * t, rmin, rmax);
	else
		return circ_sub(lhs, b * t, rmin, rmax);
}


// Override base class with your custom functionality
class FollowMe : public olc::PixelGameEngine
{
public:
	FollowMe()
	{
		// Name your application
		sAppName = "Follow me";
	}



	struct unit
	{
		float fHeading;
		olc::vf2d vPosition;
		olc::vf2d vTarget;

	};


	olc::vf2d vTarget;
	float fTime = 0.0;
	olc::Renderable Arrow;
	std::vector<unit> vecUnits;
	bool bCirularNNumbers = true;



public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		int nUnits = 60;
		for (int i = 0; i < nUnits; i++) {
			//draw in random pos
			vecUnits.push_back({ 0.0f, olc::vf2d(rand() % ScreenWidth(), rand() % ScreenHeight()) });
			
		}
		Arrow.Load("C:\\Users\\ahmad\\OneDrive\\Pictures\\download.png ", nullptr, true);
		//C:\Users\ahmad\source\repos\Follow me\Follow me\Black512.png 
		//"C:\\Users\\ahmad\\OneDrive\\Pictures\\Black.png"
		//"C:\\Users\\ahmad\\OneDrive\\Pictures\\download.png"
		


		return true;

		
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		Clear(olc::BLACK);
		if (GetMouse(0).bHeld) {
			vTarget = GetMousePos();
			fTime = 0;
		}
		if (GetKey(olc::Key::SPACE).bPressed) {
			bCirularNNumbers = !bCirularNNumbers;
			
		}
		fTime += fElapsedTime;
		for (auto& unit : vecUnits)
		{
			float fAngleToNewTarget = atan2(unit.vPosition.y - vTarget.y, unit.vPosition.x - vTarget.x) + 3.14159f;


			float delta = circ_shortdiff(unit.fHeading, fAngleToNewTarget, 0.0f, 2.0f * 3.14159f);
			unit.fHeading = circ_add(unit.fHeading, delta * fElapsedTime * 10.0f, 0.0f, 2.0f * 3.14159f);

			
			DrawRotatedDecal(unit.vPosition, Arrow.Decal(), unit.fHeading, { 256, 256 }, { 0.039f, 0.039f });

		}

		FillCircle(vTarget, 3, olc::RED);



		return true;
	}
};

int main()
{
	FollowMe demo;
	
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}