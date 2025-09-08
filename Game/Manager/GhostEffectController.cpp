#include "GhostEffectController.h"

GhostEffectController::GhostEffectController(){}

GhostEffectController::~GhostEffectController(){}

void GhostEffectController::CreateGhostEffect(const Vector2Int& _index){
	// すでに存在している場合は作成しない
	auto itr = ghostEffects_.find(_index);
	if(itr != ghostEffects_.end()){
		return;
	}

	// 新規作成
	ghostEffects_[_index] = std::make_unique<GhostEffect>(_index);
	ghostEffects_[_index]->Start();
}

void GhostEffectController::DeleteGhostEffectByPlayer(const Vector2Int& _index){
	// 存在しない場合は何もしない
	auto itr = ghostEffects_.find(_index);

	if(itr == ghostEffects_.end()){
		return;
	}

	itr->second->DeleteByPlayer();
}

void GhostEffectController::DeleteGhostEffectByBlock(const Vector2Int& _index){
	// 存在しない場合は何もしない
	auto itr = ghostEffects_.find(_index);

	if(itr == ghostEffects_.end()){
		return;
	}

	itr->second->DeleteByBlock();
}

void GhostEffectController::Init(StageRegistry* _stageRegistry){
	pStageRegistry_ = _stageRegistry;

	if(!ghostEffects_.empty()){
		ghostEffects_.clear();
	}
}

void GhostEffectController::Update(){
	if(pStageRegistry_ == nullptr){
		return;
	}

	// state がNONEのエフェクトを削除
	// stateがNONEでないエフェクトはUpdateを呼び出す
	for(auto itr = ghostEffects_.begin(); itr != ghostEffects_.end(); ){
		if(itr->second == nullptr || itr->second->GetState() == GhostEffect::State::NONE){
			itr = ghostEffects_.erase(itr);
		} else{
			// ブロックがGhostでない場合はDeleteByBlockを呼び出す
			auto block = pStageRegistry_->GetStageDataRef()[itr->first.y][itr->first.x].get();
			if(block == nullptr || block->GetType() != BlockType::Ghost){
				itr->second->DeleteByBlock();

				++itr;
				continue;
			}

			itr->second->Update();
			++itr;
		}
	}
}

void GhostEffectController::Finalize(){
	if(!ghostEffects_.empty()){
		ghostEffects_.clear();
	}
	pStageRegistry_ = nullptr;
}