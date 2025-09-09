#include "StageSelectCollection.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Game/UI/StageSelector.h"

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

void StageSelectCollection::Update(float _scrollT, float _currentIndexT) {
	// スクロールの進行度合いに応じて位置を更新
	float diffScroll = (_scrollT - _currentIndexT);
	// 小数部分を取り出す
	diffScroll -= static_cast<float>(static_cast<int>(diffScroll));

	for (int i = 0; i < kMax; ++i) {
		float x = centerPositions_[i].x - slotSpacing_ * diffScroll;
		segments_[i]->SetCenterPosX(x);
	}
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
