
#define BEHAVIOUR_MOVE_LEFT(mutable_parameter, limit_left, keyboard, key, synch_frame) \
    if(keyboard__contains_key_code(keyboard , key) != 0){\
        mutable_parameter-=1;\
        if(mutable_parameter<limit_left){\
            mutable_parameter=limit_left;\
        }\
        synch_frame=frame;\
    }

#define BEHAVIOUR_MOVE_RIGHT(mutable_parameter, limit_right,keyboard, key, synch_frame) \
    if(keyboard__contains_key_code(kb , key) != 0){\
        mutable_parameter+=1;\
        if(mutable_parameter>limit_right){\
            mutable_parameter=limit_right;\
        }\
        synch_frame=frame;\
    }

#define BEHAVIOUR_FIRE_PROJECTILE(figure, offset_x,offset_y, keyboard, key, life0,power0,team0,synch_frame,bullet_behaviour) \
        if(keyboard__contains_key_code(kb,key ) != 0){\
            if(frame - synch_frame > 3){\
                entity_t* bullet = entity__new(frame, offset_x, offset_y,figure ,bullet_behaviour,5,0);\
                bullet->life=life0;\
                bullet->power=power0;\
                bullet->team=team0;\
                scene_manager__add_entity(manager, bullet);\
                synch_frame=frame;\
            }\
        }