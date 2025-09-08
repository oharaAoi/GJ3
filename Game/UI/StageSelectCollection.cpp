#include "StageSelectCollection.h"

void StageSelectCollection::Init(Canvas2d* _canvas2d) {
	segments_.resize(kMax);
	for (uint32_t oi = 0; oi < segments_.size(); oi++) {
		segments_[oi] = std::make_unique<StageSelectSegment>();
		segments_[oi]->Init(_canvas2d, oi);
	}
}

void StageSelectCollection::Update() {

}