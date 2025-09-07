#pragma once
#include <map>
#include <vector>
#include <string>
#include "Engine/Lib/Math/Vector3.h"
#include "Engine/Lib/Math/Quaternion.h"

#include "Engine/Lib/Json/IJsonConverter.h"

template <typename tValue>
struct Keyframe{
	float time;
	tValue value;
};
// 一旦 ここ
inline void to_json(json& j,const Keyframe<float>& kf){
	j = json{{"time",kf.time},{"value",kf.value}};
}
inline void from_json(const json& j,Keyframe<float>& kf){
	j.at("time").get_to(kf.time);
	j.at("value").get_to(kf.value);
}

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template <typename tValue>
struct AnimationCurve{
	std::vector<Keyframe<tValue>> keyframes;
};

/// <summary>
/// Nodeごとのアニメーション
/// </summary>
struct NodeAnimation{
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

struct Animation{
	float duration;		// アニメーション全体の尺
	// NodeAnimationの集合。Node名で引けるようにしておく
	std::map<std::string,NodeAnimation> nodeAnimations;
	std::string animationName;
};