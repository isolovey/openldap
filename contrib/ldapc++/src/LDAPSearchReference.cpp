/*
 * Copyright 2000, OpenLDAP Foundation, All Rights Reserved.
 * COPYING RESTRICTIONS APPLY, see COPYRIGHT file
 */


#include <iostream>

#include "debug.h"
#include "LDAPSearchReference.h"
#include "LDAPException.h"
#include "LDAPRequest.h"
#include "LDAPUrl.h"

LDAPSearchReference::LDAPSearchReference(const LDAPRequest *req,
        LDAPMessage *msg) : LDAPMsg(msg){
    DEBUG(LDAP_DEBUG_CONSTRUCT,
            "LDAPSearchReference::LDAPSearchReference()" << endl;)    
    char **ref=0;
    LDAPControl** srvctrls=0;
    const LDAPAsynConnection* con=req->getConnection();
    int err = ldap_parse_reference(con->getSessionHandle(), msg, &ref, 
            &srvctrls,0);
    if (err != LDAP_SUCCESS){
        ldap_value_free(ref);
        ldap_controls_free(srvctrls);
        throw LDAPException(err);
    }else{
        m_urlList=LDAPUrlList(ref);
        ldap_value_free(ref);
        if (srvctrls){
            m_srvControls = LDAPControlSet(srvctrls);
            m_hasControls = true;
            ldap_controls_free(srvctrls);
        }else{
            m_hasControls = false;
        }
    }
}

LDAPSearchReference::~LDAPSearchReference(){
    DEBUG(LDAP_DEBUG_DESTROY,"LDAPSearchReference::~LDAPSearchReference()"
            << endl);
}

const LDAPUrlList& LDAPSearchReference::getUrls() const{
    DEBUG(LDAP_DEBUG_TRACE,"LDAPSearchReference::getUrls()" << endl);
    return m_urlList;
}

