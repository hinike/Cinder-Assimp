
#include "ModelIo.h"
#include "Node.h"

#include "assimp/material.h"

using namespace std;
using namespace ci;

namespace cinder {
	
	Material::Material()
	: mAmbient( ColorA( 1.0f, 0.5f, 0.25f, 1.0f ) ), mDiffuse( ColorA( 0, 0, 0, 1 ) ), mSpecular( ColorA( 0, 0, 0, 1 ) ), mShininess( 1.0f ), mEmission( ColorA( 0, 0, 0, 1 ) ), mFace( GL_FRONT )
	{
	}
	
	Material::Material( const ColorA &aAmbient, const ColorA &aDiffuse, const ColorA &aSpecular,
					   float aShininess, const ColorA &aEmission, GLenum aFace )
	: mAmbient( aAmbient ), mDiffuse( aDiffuse ), mSpecular( aSpecular ), mShininess( aShininess ), mEmission( aEmission ), mFace( aFace )
	{
	}
	
} // namespace

using namespace model;

void Weights::addWeight( const NodeRef& bone, float weight ) {
	if( mActiveNumWeights == NB_WEIGHTS ) {
		// TODO: we should warn the user here?
		return;
	} else {
		mWeights[mActiveNumWeights] = weight;
		mBones[mActiveNumWeights] = bone.get();
		++mActiveNumWeights;
		
		// If we have NB_WEIGHTS bone weights and their sum isn't 1.0,
		// we renormalize the weights.
		if( mActiveNumWeights == NB_WEIGHTS ) {
			float sum = 0.0f;
			for (int i=0; i < NB_WEIGHTS; ++i) {
				sum += mWeights[i];
			}
			
			if( sum < 1.0f - EPSILON && sum !=0 ) {
				for (int i=0; i < NB_WEIGHTS; ++i) {
					mWeights[i] *= 1.0f / sum;
				}
			}
		}
	}
}
	
size_t SectionSource::getNumIndices() const
{
	return mIndices.size();
}

size_t SectionSource::getNumVertices() const
{
	return mPositions.size();
}

geom::Primitive	SectionSource::getPrimitive() const
{
	return geom::Primitive::TRIANGLES;
}
	
bool SectionSource::hasAttrib( geom::Attrib attr ) const
{
	switch( attr ) {
		case geom::Attrib::POSITION: return ! mPositions.empty(); break;
		case geom::Attrib::COLOR: return ! mColors.empty(); break;
		case geom::Attrib::TEX_COORD_0: return ! mTexCoords.empty(); break;
		case geom::Attrib::NORMAL: return ! mNormals.empty(); break;
		case geom::Attrib::TANGENT: return ! mTangents.empty(); break;
		case geom::Attrib::BITANGENT: return ! mBitangents.empty(); break;
		case geom::Attrib::BONE_INDEX: return  ! mBoneIndices.empty(); break;
		case geom::Attrib::BONE_WEIGHT: return ! mBoneWeights.empty(); break;
		case geom::Attrib::CUSTOM_0: return mMorphOffsets.size() > 0 && !mMorphOffsets.at( 0 ).empty(); break;
		case geom::Attrib::CUSTOM_1: return mMorphOffsets.size() > 1 && !mMorphOffsets.at( 1 ).empty(); break;
		case geom::Attrib::CUSTOM_2: return mMorphOffsets.size() > 2 && !mMorphOffsets.at( 2 ).empty(); break;
		case geom::Attrib::CUSTOM_3: return mMorphOffsets.size() > 3 && !mMorphOffsets.at( 3 ).empty(); break;
		case geom::Attrib::CUSTOM_4: return mMorphOffsets.size() > 4 && !mMorphOffsets.at( 4 ).empty(); break;
		case geom::Attrib::CUSTOM_5: return mMorphOffsets.size() > 5 && !mMorphOffsets.at( 5 ).empty(); break;
		case geom::Attrib::CUSTOM_6: return mMorphOffsets.size() > 6 && !mMorphOffsets.at( 6 ).empty(); break;
		case geom::Attrib::CUSTOM_7: return mMorphOffsets.size() > 7 && !mMorphOffsets.at( 7 ).empty(); break;
		case geom::Attrib::CUSTOM_8: return mMorphOffsets.size() > 8 && !mMorphOffsets.at( 8 ).empty(); break;
		case geom::Attrib::CUSTOM_9: return mMorphOffsets.size() > 9 && !mMorphOffsets.at( 9 ).empty(); break;
		default:
			return false;
	}
}

uint8_t SectionSource::getAttribDims( geom::Attrib attr ) const
{
	if( ! hasAttrib( attr ) )
		return 0;
	
	switch( attr ) {
		case geom::Attrib::POSITION: return 3;
		case geom::Attrib::COLOR: return 3;
		case geom::Attrib::TEX_COORD_0: return 2;
		case geom::Attrib::NORMAL: return 3;
		case geom::Attrib::TANGENT: return 3;
		case geom::Attrib::BITANGENT: return 3;
		case geom::Attrib::BONE_INDEX: return 4;
		case geom::Attrib::BONE_WEIGHT: return 4;
		case geom::Attrib::CUSTOM_0: return 3;
		case geom::Attrib::CUSTOM_1: return 3;
		case geom::Attrib::CUSTOM_2: return 3;
		case geom::Attrib::CUSTOM_3: return 3;
		case geom::Attrib::CUSTOM_4: return 3;
		case geom::Attrib::CUSTOM_5: return 3;
		case geom::Attrib::CUSTOM_6: return 3;
		case geom::Attrib::CUSTOM_7: return 3;
		case geom::Attrib::CUSTOM_8: return 3;
		case geom::Attrib::CUSTOM_9: return 3;
		default:
			return 0;
	}
}
	
void SectionSource::loadInto( geom::Target *target, const geom::AttribSet &requestedAttribs ) const
{
	// copy attributes
	if( getAttribDims( geom::Attrib::POSITION ) )
		target->copyAttrib( geom::Attrib::POSITION, getAttribDims( geom::Attrib::POSITION ), 0, (const float*)mPositions.data(), getNumVertices() );
	if( getAttribDims( geom::Attrib::COLOR ) )
		target->copyAttrib( geom::Attrib::COLOR, getAttribDims( geom::Attrib::COLOR ), 0, (const float*)mColors.data(), min( mColors.size(), mPositions.size() ) );
	if( getAttribDims( geom::Attrib::TEX_COORD_0 ) )
		target->copyAttrib( geom::Attrib::TEX_COORD_0, getAttribDims( geom::Attrib::TEX_COORD_0 ), 0, (const float*)mTexCoords.data(), min( mTexCoords.size(), mPositions.size() ) );
	if( getAttribDims( geom::Attrib::NORMAL ) )
		target->copyAttrib( geom::Attrib::NORMAL, getAttribDims( geom::Attrib::NORMAL ), 0, (const float*)mNormals.data(), min( mNormals.size(), mPositions.size() ) );
	if( getAttribDims( geom::Attrib::TANGENT ) )
		target->copyAttrib( geom::Attrib::TANGENT, getAttribDims( geom::Attrib::TANGENT ), 0, (const float*)mTangents.data(), min( mTangents.size(), mPositions.size() ) );
	if( getAttribDims( geom::Attrib::BITANGENT ) )
		target->copyAttrib( geom::Attrib::BITANGENT, getAttribDims( geom::Attrib::BITANGENT ), 0, (const float*)mBitangents.data(), min( mBitangents.size(), mPositions.size() ) );
	if( getAttribDims( geom::Attrib::BONE_INDEX ) )
		target->copyAttrib( geom::Attrib::BONE_INDEX, getAttribDims( geom::Attrib::BONE_INDEX ), 0, (const float*)mBoneIndices.data(), min( mBoneIndices.size(), mPositions.size() ) );
	if( getAttribDims( geom::Attrib::BONE_WEIGHT ) )
		target->copyAttrib( geom::Attrib::BONE_WEIGHT, getAttribDims( geom::Attrib::BONE_WEIGHT ), 0, (const float*)mBoneWeights.data(), min( mBoneWeights.size(), mPositions.size() ) );
	// copy indices
	if( getNumIndices() )
		target->copyIndices( geom::Primitive::TRIANGLES, mIndices.data(), getNumIndices(), 4 /* bytes per index */ );

	for( int i = 0; i < 10; ++i ) {
		auto attrib = static_cast<geom::Attrib>( size_t( geom::Attrib::CUSTOM_0 ) + i );
		if( getAttribDims( attrib ) )
			target->copyAttrib( attrib, getAttribDims( attrib ), 0, (const float*)mMorphOffsets.at( i ).data(), min( mMorphOffsets.at( i ).size(), mPositions.size() ) );
	}
}

geom::AttribSet SectionSource::getAvailableAttribs() const
{
	geom::AttribSet result;
	
	if( ! mPositions.empty() )
		result.insert( geom::Attrib::POSITION );
	if( ! mNormals.empty() )
		result.insert( geom::Attrib::NORMAL );
	if( ! mTangents.empty() )
		result.insert( geom::Attrib::TANGENT );
	if( ! mBitangents.empty() )
		result.insert( geom::Attrib::BITANGENT );
	if( ! mTexCoords.empty() )
		result.insert( geom::Attrib::TEX_COORD_0 );
	if( ! mColors.empty() )
		result.insert( geom::Attrib::COLOR );
	if( ! mBoneIndices.empty() )
		result.insert( geom::Attrib::BONE_INDEX );
	if( ! mBoneWeights.empty() )
		result.insert( geom::Attrib::BONE_WEIGHT );
	
	return result;
}

ModelIoException::ModelIoException( const string &message ) throw()
//: ModelIoException() no constructor delegation in VS2012 :(
{
	mMessage = message;
}

