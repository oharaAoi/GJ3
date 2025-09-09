#include "StageSelectCollection.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Game/UI/StageSelect/StageSelector.h"

void StageSelectCollection::Init(Canvas2d* _canvas2d) {
	SetName("StageSelectCollection");
	segments_.resize(kMax);
	centerPositions_.clear();
	centerPositions_.resize(kMax);
	for (int oi = 0; oi < segments_.size(); oi++) {
		segments_[oi] = std::make_unique<StageSelectSegment>();
		segments_[oi]->Init(_canvas2d, oi);
		centerPositions_[oi] = segments_[oi]->GetCenterPos();
		AddChild(segments_[oi].get());
	}

	//// □123□のようにしスクロールのための座標を代入する
	//centerPositions_.insert(centerPositions_.begin(), centerPositions_[0] - (kWindowWidth_ * 0.5f));
	//centerPositions_.push_back(centerPositions_.back() + (kWindowWidth_ * 0.5f));

	EditorWindows::AddObjectWindow(this, GetName());

	slotSpacing_ = 1280.0f;
}

void StageSelectCollection::Update(float _currentOffset) {
	// 3枚を配置
	// 左
	segments_[0]->SetCenterPosX(centerPositions_[1].x + _currentOffset - slotSpacing_);
	// 中央
	segments_[1]->SetCenterPosX(centerPositions_[1].x + _currentOffset);
	// 右
	segments_[2]->SetCenterPosX(centerPositions_[1].x + _currentOffset + slotSpacing_);
}

void StageSelectCollection::Debug_Gui() {
}

RenderTargetType StageSelectCollection::GetRenderTarget(uint32_t index) {
	return ptrs[index]->GetRenderTargetType();
}

void StageSelectCollection::SortSegments() {
	ptrs.clear();
	ptrs.reserve(segments_.size());
	for (auto& seg : segments_) {
		ptrs.push_back(seg.get());
	}

	// ポインタの参照先の centerPosX を基準にソート
	std::sort(ptrs.begin(), ptrs.end(),
			  [](StageSelectSegment* a, StageSelectSegment* b) {
				  return a->GetCenterPos().x < b->GetCenterPos().x;
			  });
}
