#include <Siv3D.hpp>

Vec2 WrapPosition(const Vec2& position, const Vec2& spacing)
{
	const double screenWidth = Scene::Width();
	const double screenHeight = Scene::Height();

	double wrappedX = position.x;
	double wrappedY = position.y;

	if (position.x + spacing.x < 0)
		wrappedX = screenWidth + spacing.x;
	else if (position.x - spacing.x > screenWidth)
		wrappedX = -spacing.x;

	if (position.y + spacing.y < 0)
		wrappedY = screenHeight + spacing.y;
	else if (position.y - spacing.y > screenHeight)
		wrappedY = -spacing.y;

	return Vec2(wrappedX, wrappedY);
}

void Main()
{
    // 分割数を設定
    constexpr int32 horizontalDivisions = 5;
    constexpr int32 verticalDivisions = 4;

    // 分割幅・高さを計算
    const double divisionWidth = Scene::Size().x / (horizontalDivisions - 1);
    const double divisionHeight = Scene::Size().y / (verticalDivisions - 1);

    // Circleの半径を計算
	double circleRadius = (divisionWidth / 2.0) * 0.7;

	// 格子状に並べる位置を計算
	Array<Vec2> dotPositions;
	for (int32 y = 0; y <= verticalDivisions; ++y)
	{
		for (int32 x = 0; x <= horizontalDivisions; ++x)
		{
			dotPositions.emplace_back(divisionWidth * x, divisionHeight * y);
		}
	}

	float speed = Random(300, 450);
	Array<Vec2> directions{ Vec2{1, 1}, Vec2{-1, 1}, Vec2{1, -1}, Vec2{-1, -1} };
	Vec2 direction = directions.choice().setLength(speed);
	uint32 answerIndex = Random<uint32>(0u, dotPositions.size() - 1);

    while (System::Update())
	{
		for (auto i = 0u; i < dotPositions.size(); ++i)
		{
			auto pos = dotPositions[i];
			Color color = Palette::Orange;

			if (i == answerIndex)
			{
				auto rect = RectF{ Arg::center(pos), circleRadius, circleRadius }.rotated(45_deg);
				// クリックしたら再抽選
				if (rect.leftClicked())
				{
					speed = Random(300, 450);
					direction = directions.choice().setLength(speed);
					answerIndex = Random<uint32>(0u, dotPositions.size());
				}

				rect.draw(rect.mouseOver() ? Palette::Red : color);
			}

			else
			{
				Circle(pos, circleRadius).draw(color);
			}


		}

		for (auto& pos : dotPositions)
		{
			// 位置更新
			pos += (direction * Scene::DeltaTime());

			// 画面外にはみ出たら反対側から出す
			pos = WrapPosition(pos, Vec2{ divisionWidth, divisionHeight });
		}
	}
}
