#include "pch.h"
#include "BaseScene.h"
#include "SceneCommons.h"
#include <Framework/SceneManager.h>
#include <Framework/DebugTools/DebugCameraWrapper.h>
#include <Framework/DebugTools/GridFloorWrapper.h>
#include <Utilities/Input.h>
#include <Utilities/picojson.h>
#include <fstream>
#include <sstream>

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace picojson
{
	template <typename T> T get_as(value& v);
	template <> Vector3 get_as<Vector3>(value& v) {
		auto& vec = v.get<array>();
		return Vector3{ float(vec[0].get<double>()), float(vec[1].get<double>()), float(vec[2].get<double>()) };
	}
}

// 生成
void BaseScene::Build(GameContext& context)
{
	auto debugCamera = std::make_shared<DebugCameraWrapper>();
	context << debugCamera;
	context << std::make_shared<GridFloorWrapper>();

	context.GetCamera().view = Matrix::CreateLookAt(Vector3(0, 5, 10), Vector3::Zero, Vector3::Up);
	debugCamera->GetDebugCamera().m_xAngle = XMConvertToRadians(60);

	{
		static std::unordered_map<std::string, XMVECTOR> colors = 
		{
			{ "red", Colors::Red },
			{ "blue", Colors::Blue },
			{ "green", Colors::Green },
			{ "white", Colors::White },
			{ "black", Colors::Black },
		};
		static std::unordered_map<std::string, std::function<std::unique_ptr<DirectX::GeometricPrimitive>(GameContext& context)>> types =
		{
			{ "cube", [](GameContext& context) { return GeometricPrimitive::CreateCube(context.GetDR().GetD3DDeviceContext()); } },
			{ "box", [](GameContext& context) { return GeometricPrimitive::CreateCube(context.GetDR().GetD3DDeviceContext()); } },
			{ "circle", [](GameContext& context) { return GeometricPrimitive::CreateSphere(context.GetDR().GetD3DDeviceContext()); } },
		};

		// JSONファイルの読み込み
		std::ifstream ifs("Resources/Scenes/object.json");
		if (ifs.fail())
			throw std::exception("failed to read object.json");

		// JSONデータの読み込み
		picojson::value value;
		ifs >> value;
		ifs.close();

		// 確認用
		std::cout << value << std::endl;

		// データの使用
		picojson::object& root = value.get<picojson::object>();

		std::cout << std::endl;

		// オブジェクトの要素を順に処理
		for (auto& entry : root)
		{
			auto name = entry.first;
			auto& obj = entry.second.get<picojson::object>();

			auto& type = obj["type"].get<std::string>();
			auto position = picojson::get_as<Vector3>(obj["position"]);
			auto size = picojson::get_as<Vector3>(obj["size"]);
			auto& color = obj["color"].get<std::string>();
			auto vel = Vector3::Zero;
			if (obj.count("vel"))
				vel = picojson::get_as<Vector3>(obj["vel"]);

			auto geo = std::make_shared<GeometricObject>(types[type], colors[color]);
			geo->transform.LocalPosition = position;
			geo->transform.LocalScale = size;
			context << geo;
		}
	}
}
