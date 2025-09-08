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

	// □123□のようにしスクロールのための座標を代入する
	centerPositions_.insert(centerPositions_.begin(), centerPositions_[0] - (kWindowWidth_ * 0.5f));
	centerPositions_.push_back(centerPositions_.back() + (kWindowWidth_ * 0.5f));

	EditorWindows::AddObjectWindow(this, GetName());

	scrollPos_ = 0;
	slotSpacing_ = 1280.0f;
}

void StageSelectCollection::Update(float _scrolT, int32_t _scrollDirection) {
	scrollPos_ = 0;
	if (_scrollDirection < 0) {
		scrollPos_ += _scrolT;
	} else if(_scrollDirection > 0) {
		scrollPos_ -= _scrolT;
	}

	for (int i = 0; i < kMax; ++i) {
		float x = centerPositions_[i + 1].x - slotSpacing_ * scrollPos_;
		segments_[i]->SetCenterPosX(x);
	}

	while (scrollPos_ >= 1.0f) {
		scrollPos_ -= 1.0f;
		// 先頭を末尾へ回す等：segments_をローテートし、見かけ上の並びを保つ
		std::rotate(segments_.begin(), segments_.begin() + 1, segments_.end());
	}
	while (scrollPos_ <= -1.0f) {
		scrollPos_ += 1.0f;
		std::rotate(segments_.rbegin(), segments_.rbegin() + 1, segments_.rend());
	}

	////// 左方向にスクロールさせる(ステージのindexを上げる)
	////if (_scrolT == 0.0f) { return; }

	////if (_scrollDirection < 0) {
	////	// centerPositions_配列の頭と穴以外の座標をスクロールさせる
	////	for (int oi = 1; oi < centerPositions_.size() - 1; ++oi) {
	////		float newX = std::lerp(centerPositions_[oi].x, centerPositions_[oi - 1].x, _scrolT);
	////		segments_[oi - 1]->SetCenterPosX(newX);
	////	}

	////	// tが1を超えたら一番左のステ－ジ内容を一番右に持ってくる
	////	if (_scrolT >= 1) {
	////		segments_[0]->SetCenterPosX(centerPositions_[kMax + 1].x);
	////	}

	////	// 右方向にスクロールさせる(ステージのindexを下げる)
	////} else if (_scrollDirection > 0) {
	////	// centerPositions_配列の頭と穴以外の座標をスクロールさせる
	////	for (int oi = 1; oi < centerPositions_.size() - 1; ++oi) {
	////		float newX = std::lerp(centerPositions_[oi].x, centerPositions_[oi + 1].x, _scrolT);
	////		segments_[oi - 1]->SetCenterPosX(newX);
	////	}

	////	// tが1を超えたら一番右のステ－ジ内容を一番左に持ってくる
	////	if (_scrolT >= 1) {
	////		segments_[kMax - 1]->SetCenterPosX(centerPositions_[0].x);
	////	}
	////}
}

void StageSelectCollection::Debug_Gui() {
}

RenderTargetType StageSelectCollection::GetRenderTarget(uint32_t index) {
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
	return ptrs[index]->GetRenderTargetType();
}
