/*
 * Copyright 1998-1999 The OpenLDAP Foundation, All Rights Reserved.
 * COPYING RESTRICTIONS APPLY, see COPYRIGHT file
 */
#include "portable.h"

#include <stdlib.h>
#include <ac/string.h>

#include "lber-int.h"

struct lber_options ber_int_options = {
	LBER_UNINITIALIZED, 0, 0 };

int
ber_get_option(
	LDAP_CONST void	*item,
	int		option,
	void	*outvalue)
{
	LDAP_CONST BerElement *ber;
	LDAP_CONST Sockbuf *sb;

	ber_int_options.lbo_valid = LBER_INITIALIZED;

	if(outvalue == NULL) {
		/* no place to get to */
		return LBER_OPT_ERROR;
	}

	if(item == NULL) {
		if(option == LBER_OPT_BER_DEBUG) {
			* (int *) outvalue = ber_int_debug;
			return LBER_OPT_SUCCESS;
		}

		return LBER_OPT_ERROR;
	}

	ber = item;
	sb = item;

	switch(option) {
	case LBER_OPT_BER_OPTIONS:
		assert( BER_VALID( ber ) );
		* (int *) outvalue = ber->ber_options;
		return LBER_OPT_SUCCESS;

	case LBER_OPT_BER_DEBUG:
		assert( BER_VALID( ber ) );
		* (int *) outvalue = ber->ber_debug;
		return LBER_OPT_SUCCESS;

	default:
		/* bad param */
		break;
	}

	return LBER_OPT_ERROR;
}

int
ber_set_option(
	void	*item,
	int		option,
	LDAP_CONST void	*invalue)
{
	BerElement *ber;
	Sockbuf *sb;

	if( (ber_int_options.lbo_valid == LBER_UNINITIALIZED)
		&& ( ber_int_memory_fns == NULL )
		&& ( option == LBER_OPT_MEMORY_FNS )
		&& ( invalue != NULL ))
	{
		BerMemoryFunctions *f = (BerMemoryFunctions *) invalue;

		/* make sure all functions are provided */
		if(!( f->bmf_malloc && f->bmf_calloc
			&& f->bmf_realloc && f->bmf_free ))
		{
			return LBER_OPT_ERROR;
		}

		ber_int_memory_fns = (BerMemoryFunctions *)
			(*(f->bmf_malloc))(sizeof(BerMemoryFunctions));

		if ( ber_int_memory_fns == NULL ) {
			return LBER_OPT_ERROR;
		}

		memcpy(ber_int_memory_fns, f, sizeof(BerMemoryFunctions));

		ber_int_options.lbo_valid = LBER_INITIALIZED;
		return LBER_OPT_SUCCESS;
	}

	ber_int_options.lbo_valid = LBER_INITIALIZED;

	if(invalue == NULL) {
		/* no place to set from */
		return LBER_OPT_ERROR;
	}

	if(item == NULL) {
		if(option == LBER_OPT_BER_DEBUG) {
			ber_int_debug = * (int *) invalue;
			return LBER_OPT_SUCCESS;

		} else if(option == LBER_OPT_LOG_PRINT_FN) {
			ber_pvt_log_print = (BER_LOG_PRINT_FN) invalue;
			return LBER_OPT_SUCCESS;
		}

		return LBER_OPT_ERROR;
	}

	ber = item;
	sb = item;

	switch(option) {
	case LBER_OPT_BER_OPTIONS:
		assert( BER_VALID( ber ) );
		ber->ber_options = * (int *) invalue;
		return LBER_OPT_SUCCESS;

	case LBER_OPT_BER_DEBUG:
		assert( BER_VALID( ber ) );
		ber->ber_debug = * (int *) invalue;
		return LBER_OPT_SUCCESS;

	default:
		/* bad param */
		break;
	}

	return LBER_OPT_ERROR;
}
