/*
 *  ParticleSystemManager.mm
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 4/26/15.
 *  Copyright 2011-2015 mousebird consulting. All rights reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#import "ParticleSystemManager.h"
#import "ParticleSystemDrawable.h"

namespace WhirlyKit
{

ParticleSystemSceneRep::ParticleSystemSceneRep()
{
}

ParticleSystemSceneRep::ParticleSystemSceneRep(SimpleIdentity inId)
: Identifiable(inId)
{
}
    
ParticleSystemSceneRep::~ParticleSystemSceneRep()
{
}
    
void ParticleSystemSceneRep::clearContents(ChangeSet &changes)
{
    for (const ParticleSystemDrawable *it : draws)
        changes.push_back(new RemDrawableReq(it->getId()));
}
    
void ParticleSystemSceneRep::enableContents(bool enable,ChangeSet &changes)
{
    for (const ParticleSystemDrawable *it : draws)
        changes.push_back(new OnOffChangeRequest(it->getId(),enable));
}
    
ParticleSystemManager::ParticleSystemManager()
{
    pthread_mutex_init(&partSysLock, NULL);
}
    
ParticleSystemManager::~ParticleSystemManager()
{
    pthread_mutex_destroy(&partSysLock);
    for (auto it : sceneReps)
        delete it;
    sceneReps.clear();
}
    
SimpleIdentity ParticleSystemManager::addParticleSystem(const ParticleSystem &newSystem,ChangeSet &changes)
{
    ParticleSystemSceneRep *sceneRep = new ParticleSystemSceneRep();

    sceneRep->partSys = newSystem;
    
    SimpleIdentity partSysID = sceneRep->getId();
    
    pthread_mutex_lock(&partSysLock);
    sceneReps.insert(sceneRep);
    pthread_mutex_unlock(&partSysLock);
    
    return partSysID;
}
    
void ParticleSystemManager::enableParticleSystem(SimpleIdentity sysID,bool enable,ChangeSet &changes)
{
    pthread_mutex_lock(&partSysLock);
    
    ParticleSystemSceneRep dummyRep(sysID);
    auto it = sceneReps.find(&dummyRep);
    if (it != sceneReps.end())
        (*it)->enableContents(enable, changes);
    
    pthread_mutex_unlock(&partSysLock);
}
    
void ParticleSystemManager::removeParticleSystem(SimpleIdentity sysID,ChangeSet &changes)
{
    pthread_mutex_lock(&partSysLock);
    
    ParticleSystemSceneRep dummyRep(sysID);
    auto it = sceneReps.find(&dummyRep);
    if (it != sceneReps.end())
    {
        (*it)->clearContents(changes);
        sceneReps.erase(it);
    }
    
    pthread_mutex_unlock(&partSysLock);
}
    
void ParticleSystemManager::addParticleBatch(SimpleIdentity sysID,const ParticleBatch &batch,ChangeSet &changes)
{
    pthread_mutex_lock(&partSysLock);
    
    NSTimeInterval now = CFAbsoluteTimeGetCurrent();
    
    ParticleSystemSceneRep *sceneRep = NULL;
    ParticleSystemSceneRep dummyRep(sysID);
    auto it = sceneReps.find(&dummyRep);
    if (it != sceneReps.end())
        sceneRep = *it;
    
    if (sceneRep)
    {
        ParticleSystemDrawable *draw = new ParticleSystemDrawable("Particle System",sceneRep->partSys.vertAttrs,sceneRep->partSys.batchSize);
        draw->setOnOff(true);
        draw->setPointSize(sceneRep->partSys.pointSize);
        draw->setProgram(sceneRep->partSys.shaderID);
        draw->setupGL(NULL, scene->getMemManager());
        draw->setDrawPriority(sceneRep->partSys.drawPriority);
        draw->setStartTime(now);
        draw->setLifetime(sceneRep->partSys.lifetime);
        std::vector<ParticleSystemDrawable::AttributeData> attrData;
        for (unsigned int ii=0;ii<batch.attrData.size();ii++)
        {
            ParticleSystemDrawable::AttributeData thisAttrData;
            thisAttrData.data = batch.attrData[ii];
            attrData.push_back(thisAttrData);
        }
        draw->addAttributeData(attrData);
        draw->setTexIDs(sceneRep->partSys.texIDs);
        
        changes.push_back(new AddDrawableReq(draw));
        
        sceneRep->draws.insert(draw);
    }
    
    pthread_mutex_unlock(&partSysLock);
}
    
void ParticleSystemManager::housekeeping(NSTimeInterval now,ChangeSet &changes)
{
    pthread_mutex_lock(&partSysLock);
    
    std::vector<ParticleSystemSceneRepSet::iterator> sceneRepsToRemove;
    
    for (ParticleSystemSceneRepSet::iterator it = sceneReps.begin(); it != sceneReps.end(); ++it)
    {
        std::vector<ParticleSystemDrawable *> toRemove;
        for (ParticleSystemDrawable *draw : (*it)->draws)
        {
            if (draw->getStartTime() + (*it)->partSys.lifetime < now)
                toRemove.push_back(draw);
        }
        for (auto rem : toRemove)
        {
            (*it)->draws.erase(rem);
            changes.push_back(new RemDrawableReq(rem->getId()));
        }
        
        if ((*it)->draws.empty())
            sceneRepsToRemove.push_back(it);
    }
    
    for (ParticleSystemSceneRepSet::iterator it : sceneRepsToRemove)
        sceneReps.erase(it);

    pthread_mutex_unlock(&partSysLock);
}
    
}
