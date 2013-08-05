/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2000-2003, Ximian, Inc.
 */

#ifndef SOUP_MISC_H
#define SOUP_MISC_H 1

#include <glib-object.h>

G_BEGIN_DECLS

/* Non-default-GMainContext operations */
GSource           *soup_add_io_watch         (GMainContext *async_context,
					      GIOChannel   *chan,
					      GIOCondition  condition,
					      GIOFunc       function,
					      gpointer      data);
GSource           *soup_add_idle             (GMainContext *async_context,
					      GSourceFunc   function,
					      gpointer      data);
GSource           *soup_add_completion	     (GMainContext *async_context,
					      GSourceFunc   function,
					      gpointer      data);
GSource           *soup_add_timeout          (GMainContext *async_context,
					      guint         interval,
					      GSourceFunc   function,
					      gpointer      data);

/* Misc utils */

guint              soup_str_case_hash        (gconstpointer key);
gboolean           soup_str_case_equal       (gconstpointer v1,
					      gconstpointer v2);

#define _SOUP_ATOMIC_INTERN_STRING(variable, value) ((const char *)(g_atomic_pointer_get (&(variable)) ? (variable) : (g_atomic_pointer_set (&(variable), (gpointer)g_intern_static_string (value)), (variable))))

/* SSL stuff */

extern const gboolean soup_ssl_supported;

#define SOUP_SSL_ERROR soup_ssl_error_quark()

GQuark soup_ssl_error_quark (void);

typedef enum {
	SOUP_SSL_ERROR_HANDSHAKE_NEEDS_READ,
	SOUP_SSL_ERROR_HANDSHAKE_NEEDS_WRITE,
	SOUP_SSL_ERROR_CERTIFICATE
} SoupSSLError;

typedef enum {
	SOUP_CONNECTION_NEW,
	SOUP_CONNECTION_CONNECTING,
	SOUP_CONNECTION_IDLE,
	SOUP_CONNECTION_IN_USE,
	SOUP_CONNECTION_REMOTE_DISCONNECTED,
	SOUP_CONNECTION_DISCONNECTED
} SoupConnectionState;

G_END_DECLS

#endif /* SOUP_MISC_H */
