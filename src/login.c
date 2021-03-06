#include "lua.h"
#include "lauxlib.h"
#include "compat-5.3.h"

#include <stdlib.h> /* free */
#include <stdint.h> /* uint64_t */
#include <sys/types.h> /* pid_t */
#include <errno.h> /* ENOTSUP */

#include <systemd/sd-login.h>

#include "util.c"
#include "login.h"


shim_weak_stub_declare(int, sd_pid_get_session, (pid_t pid, char **session), -ENOTSUP);
shim_weak_stub_declare(int, sd_pid_get_owner_uid, (pid_t pid, uid_t *uid), -ENOTSUP);
shim_weak_stub_declare(int, sd_pid_get_unit, (pid_t pid, char **unit), -ENOTSUP);
shim_weak_stub_declare(int, sd_pid_get_user_unit, (pid_t pid, char **unit), -ENOTSUP);
shim_weak_stub_declare(int, sd_pid_get_machine_name, (pid_t pid, char **machine), -ENOTSUP);
shim_weak_stub_declare(int, sd_pid_get_slice, (pid_t pid, char **slice), -ENOTSUP);
shim_weak_stub_declare(int, sd_peer_get_session, (int fd, char **session), -ENOTSUP);
shim_weak_stub_declare(int, sd_peer_get_owner_uid, (int fd, uid_t *uid), -ENOTSUP);
shim_weak_stub_declare(int, sd_peer_get_unit, (int fd, char **unit), -ENOTSUP);
shim_weak_stub_declare(int, sd_peer_get_user_unit, (int fd, char **unit), -ENOTSUP);
shim_weak_stub_declare(int, sd_peer_get_machine_name, (int fd, char **machine), -ENOTSUP);
shim_weak_stub_declare(int, sd_peer_get_slice, (int fd, char **slice), -ENOTSUP);
shim_weak_stub_declare(int, sd_uid_get_state, (uid_t uid, char **state), -ENOTSUP);
shim_weak_stub_declare(int, sd_uid_get_display, (uid_t uid, char **session), -ENOTSUP);
shim_weak_stub_declare(int, sd_uid_is_on_seat, (uid_t uid, int require_active, const char *seat), -ENOTSUP);
shim_weak_stub_declare(int, sd_uid_get_sessions, (uid_t uid, int require_active, char ***sessions), -ENOTSUP);
shim_weak_stub_declare(int, sd_uid_get_seats, (uid_t uid, int require_active, char ***seats), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_is_active, (const char *session), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_is_remote, (const char *session), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_state, (const char *session, char **state), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_uid, (const char *session, uid_t *uid), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_seat, (const char *session, char **seat), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_service, (const char *session, char **service), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_type, (const char *session, char **type), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_class, (const char *session, char **clazz), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_desktop, (const char *session, char **desktop), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_display, (const char *session, char **display), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_remote_host, (const char *session, char **remote_host), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_remote_user, (const char *session, char **remote_user), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_tty, (const char *session, char **display), -ENOTSUP);
shim_weak_stub_declare(int, sd_session_get_vt, (const char *session, unsigned *vtnr), -ENOTSUP);
shim_weak_stub_declare(int, sd_seat_get_active, (const char *seat, char **session, uid_t *uid), -ENOTSUP);
shim_weak_stub_declare(int, sd_seat_get_sessions, (const char *seat, char ***sessions, uid_t **uid, unsigned *n_uids), -ENOTSUP);
shim_weak_stub_declare(int, sd_seat_can_multi_session, (const char *seat), -ENOTSUP);
shim_weak_stub_declare(int, sd_seat_can_tty, (const char *seat), -ENOTSUP);
shim_weak_stub_declare(int, sd_seat_can_graphical, (const char *seat), -ENOTSUP);
shim_weak_stub_declare(int, sd_machine_get_class, (const char *machine, char **clazz), -ENOTSUP);
shim_weak_stub_declare(int, sd_machine_get_ifindices, (const char *machine, int **ifindices), -ENOTSUP);
shim_weak_stub_declare(int, sd_get_seats, (char ***seats), -ENOTSUP);
shim_weak_stub_declare(int, sd_get_sessions, (char ***sessions), -ENOTSUP);
shim_weak_stub_declare(int, sd_get_uids, (uid_t **users), -ENOTSUP);
shim_weak_stub_declare(int, sd_get_machine_names, (char ***machines), -ENOTSUP);
shim_weak_stub_declare(int, sd_login_monitor_new, (const char *category, sd_login_monitor** ret), -ENOTSUP);
shim_weak_stub_declare(sd_login_monitor*, sd_login_monitor_unref, (sd_login_monitor *m), NULL);
shim_weak_stub_declare(int, sd_login_monitor_flush, (sd_login_monitor *m), -ENOTSUP);
shim_weak_stub_declare(int, sd_login_monitor_get_fd, (sd_login_monitor *m), -ENOTSUP);
shim_weak_stub_declare(int, sd_login_monitor_get_events, (sd_login_monitor *m), -ENOTSUP);
shim_weak_stub_declare(int, sd_login_monitor_get_timeout, (sd_login_monitor *m, uint64_t *timeout_usec), -ENOTSUP);

static void push_array_of_strings (lua_State *L, char **strings, int n) {
	int i;
	lua_createtable(L, n, 0);
	if (strings != NULL) {
		for (i=0; i<n; i++) {
			lua_pushstring(L, strings[i]);
			free(strings[i]);
			lua_rawseti(L, -2, i+1);
		}
		free(strings);
	}
}

static void push_array_of_uids (lua_State *L, uid_t *uids, int n) {
	int i;
	lua_createtable(L, n, 0);
	if (uids != NULL) {
		for (i=0; i<n; i++) {
			lua_pushinteger(L, uids[i]);
			lua_rawseti(L, -2, i+1);
		}
		free(uids);
	}
}

static int get_seats (lua_State *L) {
	char **seats;
	int n = shim_weak_stub(sd_get_seats)(&seats);
	if (n < 0) return handle_error(L, -n);
	push_array_of_strings(L, seats, n);
	return 1;
}

static int get_sessions (lua_State *L) {
	char **sessions;
	int n = shim_weak_stub(sd_get_sessions)(&sessions);
	if (n < 0) return handle_error(L, -n);
	push_array_of_strings(L, sessions, n);
	return 1;
}

static int get_uids (lua_State *L) {
	uid_t *users;
	int n = shim_weak_stub(sd_get_uids)(&users);
	if (n < 0) return handle_error(L, -n);
	push_array_of_uids(L, users, n);
	return 1;
}

static int get_machine_names (lua_State *L) {
	char **machines;
	int n = shim_weak_stub(sd_get_machine_names)(&machines);
	if (n < 0) return handle_error(L, -n);
	push_array_of_strings(L, machines, n);
	return 1;
}

static int pid_get_session (lua_State *L) {
	pid_t pid = luaL_checkinteger(L, 1);
	char *session;
	int n = shim_weak_stub(sd_pid_get_session)(pid, &session);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, session);
	free(session);
	return 1;
}

static int pid_get_unit (lua_State *L) {
	pid_t pid = luaL_checkinteger(L, 1);
	char *unit;
	int n = shim_weak_stub(sd_pid_get_unit)(pid, &unit);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, unit);
	free(unit);
	return 1;
}

static int pid_get_user_unit (lua_State *L) {
	pid_t pid = luaL_checkinteger(L, 1);
	char *unit;
	int n = shim_weak_stub(sd_pid_get_user_unit)(pid, &unit);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, unit);
	free(unit);
	return 1;
}

static int pid_get_owner_uid (lua_State *L) {
	pid_t pid = luaL_checkinteger(L, 1);
	uid_t user;
	int n = shim_weak_stub(sd_pid_get_owner_uid)(pid, &user);
	if (n < 0) return handle_error(L, -n);
	lua_pushinteger(L, user);
	return 1;
}

static int pid_get_machine_name (lua_State *L) {
	pid_t pid = luaL_checkinteger(L, 1);
	char *machine;
	int n = shim_weak_stub(sd_pid_get_machine_name)(pid, &machine);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, machine);
	free(machine);
	return 1;
}

static int pid_get_slice (lua_State *L) {
	pid_t pid = luaL_checkinteger(L, 1);
	char *slice;
	int n = shim_weak_stub(sd_pid_get_slice)(pid, &slice);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, slice);
	free(slice);
	return 1;
}

static int peer_get_session (lua_State *L) {
	int fd = luaL_checkinteger(L, 1);
	char *session;
	int n = shim_weak_stub(sd_peer_get_session)(fd, &session);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, session);
	free(session);
	return 1;
}

static int peer_get_unit (lua_State *L) {
	int fd = luaL_checkinteger(L, 1);
	char *unit;
	int n = shim_weak_stub(sd_peer_get_unit)(fd, &unit);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, unit);
	free(unit);
	return 1;
}

static int peer_get_user_unit (lua_State *L) {
	int fd = luaL_checkinteger(L, 1);
	char *unit;
	int n = shim_weak_stub(sd_peer_get_user_unit)(fd, &unit);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, unit);
	free(unit);
	return 1;
}

static int peer_get_owner_uid (lua_State *L) {
	int fd = luaL_checkinteger(L, 1);
	uid_t user;
	int n = shim_weak_stub(sd_peer_get_owner_uid)(fd, &user);
	if (n < 0) return handle_error(L, -n);
	lua_pushinteger(L, user);
	return 1;
}

static int peer_get_machine_name (lua_State *L) {
	int fd = luaL_checkinteger(L, 1);
	char *machine;
	int n = shim_weak_stub(sd_peer_get_machine_name)(fd, &machine);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, machine);
	free(machine);
	return 1;
}

static int peer_get_slice (lua_State *L) {
	int fd = luaL_checkinteger(L, 1);
	char *slice;
	int n = shim_weak_stub(sd_pid_get_slice)(fd, &slice);
	if (n < 0) return handle_error(L, -n);
	lua_pushstring(L, slice);
	free(slice);
	return 1;
}

static int uid_get_state (lua_State *L) {
	uid_t uid = luaL_checkinteger(L, 1);
	char *state;
	int err = shim_weak_stub(sd_uid_get_state)(uid, &state);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, state);
	free(state);
	return 1;
}

static int uid_get_seats (lua_State *L) {
	uid_t uid = luaL_checkinteger(L, 1);
	int require_active = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	char **seats;
	int err = shim_weak_stub(sd_uid_get_seats)(uid, require_active, &seats);
	if (err < 0) return handle_error(L, -err);
	push_array_of_strings(L, seats, err);
	return 1;
}

static int uid_is_on_seat (lua_State *L) {
	uid_t uid = luaL_checkinteger(L, 1);
	int require_active = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	const char *seat = luaL_checkstring(L, 3);
	int err = shim_weak_stub(sd_uid_is_on_seat)(uid, require_active, seat);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int uid_get_sessions (lua_State *L) {
	uid_t uid = luaL_checkinteger(L, 1);
	int require_active = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));
	char ** sessions;
	int err = shim_weak_stub(sd_uid_get_sessions)(uid, require_active, &sessions);
	if (err < 0) return handle_error(L, -err);
	push_array_of_strings(L, sessions, err);
	return 1;
}

static int uid_get_display (lua_State *L) {
	uid_t uid = luaL_checkinteger(L, 1);
	char *session;
	int err = shim_weak_stub(sd_uid_get_display)(uid, &session);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, session);
	free(session);
	return 1;
}

static int session_is_active (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	int err = shim_weak_stub(sd_session_is_active)(session);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int session_is_remote (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	int err = shim_weak_stub(sd_session_is_remote)(session);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int session_get_state (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *state;
	int err = shim_weak_stub(sd_session_get_state)(session, &state);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, state);
	free(state);
	return 1;
}

static int session_get_uid (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	uid_t uid;
	int err = shim_weak_stub(sd_session_get_uid)(session, &uid);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, uid);
	return 1;
}

static int session_get_seat (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *seat;
	int err = shim_weak_stub(sd_session_get_seat)(session, &seat);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, seat);
	free(seat);
	return 1;
}

static int session_get_service (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *service;
	int err = shim_weak_stub(sd_session_get_service)(session, &service);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, service);
	free(service);
	return 1;
}

static int session_get_type (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *type;
	int err = shim_weak_stub(sd_session_get_type)(session, &type);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, type);
	free(type);
	return 1;
}

static int session_get_class (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *clazz;
	int err = shim_weak_stub(sd_session_get_class)(session, &clazz);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, clazz);
	free(clazz);
	return 1;
}

static int session_get_desktop (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *desktop;
	int err = shim_weak_stub(sd_session_get_desktop)(session, &desktop);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, desktop);
	free(desktop);
	return 1;
}

static int session_get_display (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *display;
	int err = shim_weak_stub(sd_session_get_display)(session, &display);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, display);
	free(display);
	return 1;
}

static int session_get_remote_host (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *remote_host;
	int err = shim_weak_stub(sd_session_get_remote_host)(session, &remote_host);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, remote_host);
	free(remote_host);
	return 1;
}

static int session_get_remote_user (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *remote_user;
	int err = shim_weak_stub(sd_session_get_remote_user)(session, &remote_user);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, remote_user);
	free(remote_user);
	return 1;
}

static int session_get_tty (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	char *display;
	int err = shim_weak_stub(sd_session_get_tty)(session, &display);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, display);
	free(display);
	return 1;
}

static int session_get_vt (lua_State *L) {
	const char *session = luaL_checkstring(L, 1);
	unsigned vtnr;
	int err = shim_weak_stub(sd_session_get_vt)(session, &vtnr);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, vtnr);
	return 1;
}

static int seat_get_active (lua_State *L) {
	const char *seat = luaL_optstring(L, 1, NULL);
	char *session;
	uid_t uid;
	int err = shim_weak_stub(sd_seat_get_active)(seat, &session, &uid);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, session);
	lua_pushinteger(L, uid);
	return 2;
}

static int seat_get_sessions (lua_State *L) {
	const char *seat = luaL_optstring(L, 1, NULL);
	char **sessions;
	uid_t *uid;
	unsigned n_uids;
	int err = shim_weak_stub(sd_seat_get_sessions)(seat, &sessions, &uid, &n_uids);
	if (err < 0) return handle_error(L, -err);
	push_array_of_strings(L, sessions, err);
	push_array_of_uids(L, uid, n_uids);
	return 2;
}

static int seat_can_multi_session (lua_State *L) {
	const char *seat = luaL_optstring(L, 1, NULL);
	int err = shim_weak_stub(sd_seat_can_multi_session)(seat);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int seat_can_tty (lua_State *L) {
	const char *seat = luaL_optstring(L, 1, NULL);
	int err = shim_weak_stub(sd_seat_can_tty)(seat);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int seat_can_graphical (lua_State *L) {
	const char *seat = luaL_optstring(L, 1, NULL);
	int err = shim_weak_stub(sd_seat_can_graphical)(seat);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, err);
	return 1;
}

static int machine_get_class (lua_State *L) {
	const char *machine = luaL_checkstring(L, 1);
	char *clazz;
	int err = shim_weak_stub(sd_machine_get_class)(machine, &clazz);
	if (err < 0) return handle_error(L, -err);
	lua_pushstring(L, clazz);
	free(clazz);
	return 1;
}

static int machine_get_ifindices (lua_State *L) {
	const char *machine = luaL_checkstring(L, 1);
	int *ifindices;
	int i;
	int err = shim_weak_stub(sd_machine_get_ifindices)(machine, &ifindices);
	if (err < 0) return handle_error(L, -err);
	lua_createtable(L, err, 0);
	if (ifindices != NULL) {
		for (i=0; i<err; i++) {
			lua_pushinteger(L, ifindices[i]);
			lua_rawseti(L, -2, i+1);
		}
		free(ifindices);
	}
	return 1;
}

/* sd_login_monitor */

static sd_login_monitor* check_monitor(lua_State *L, int index) {
	sd_login_monitor **mp = luaL_checkudata(L, index, MONITOR_METATABLE);
	if (*mp == NULL) luaL_error(L, "Invalid monitor handle");
	return *mp;
}

static int monitor_unref (lua_State *L) {
	sd_login_monitor **mp = luaL_checkudata(L, 1, MONITOR_METATABLE);
	if (*mp != NULL) {
		shim_weak_stub(sd_login_monitor_unref)(*mp);
		*mp = NULL;
	}
	return 0;
}

static int monitor_new (lua_State *L) {
	const char* category = luaL_optstring(L, 1, NULL);
	sd_login_monitor** ret = lua_newuserdata(L, sizeof(sd_login_monitor*));
	int err = shim_weak_stub(sd_login_monitor_new)(category, ret);
	if (err < 0) return handle_error(L, -err);
	luaL_setmetatable(L, MONITOR_METATABLE);
	return 1;
}

static int monitor_tostring (lua_State *L) {
	sd_login_monitor *m = check_monitor(L, 1);
	lua_pushfstring(L, "%s: %p", MONITOR_METATABLE, m);
	return 1;
}

static int monitor_flush (lua_State *L) {
	sd_login_monitor *m = check_monitor(L, 1);
	int err = shim_weak_stub(sd_login_monitor_flush)(m);
	if (err < 0) return handle_error(L, -err);
	lua_pushboolean(L, 1);
	return 1;
}

static int monitor_get_fd (lua_State *L) {
	sd_login_monitor *m = check_monitor(L, 1);
	int err = shim_weak_stub(sd_login_monitor_get_fd)(m);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int monitor_get_events (lua_State *L) {
	sd_login_monitor *m = check_monitor(L, 1);
	int err = shim_weak_stub(sd_login_monitor_get_events)(m);
	if (err < 0) return handle_error(L, -err);
	lua_pushinteger(L, err);
	return 1;
}

static int monitor_get_timeout (lua_State *L) {
	sd_login_monitor *m = check_monitor(L, 1);
	uint64_t timeout_usec;
	int err = shim_weak_stub(sd_login_monitor_get_timeout)(m, &timeout_usec);
	if (err < 0) return handle_error(L, -err);
	if ((err == 0) || (timeout_usec == (uint64_t) -1)) {
		lua_pushboolean(L, 0);
	} else {
		lua_pushnumber(L, ((double)timeout_usec)/1000000);
	}
	return 1;
}

int luaopen_systemd_login_core (lua_State *L) {
	lua_newtable(L);
	/* 209 */
	if (symbol_exists("sd_monitor_new") && symbol_exists("sd_login_monitor_unref")) {
		if (luaL_newmetatable(L, MONITOR_METATABLE) != 0) {
			lua_newtable(L);
			set_func_if_symbol_exists("sd_monitor_flush", L, monitor_flush, "flush");
			set_func_if_symbol_exists("sd_monitor_get_fd", L, monitor_get_fd, "get_fd");
			set_func_if_symbol_exists("sd_monitor_get_events", L, monitor_get_events,"get_events");
			set_func_if_symbol_exists("sd_monitor_get_timeout", L, monitor_get_timeout, "get_timeout");
			lua_setfield(L, -2, "__index");
			lua_pushcfunction(L, monitor_unref);
			lua_setfield(L, -2, "__gc");
			lua_pushcfunction(L, monitor_tostring);
			lua_setfield(L, -2, "__tostring");
		}
		/* Expose monitor methods */
		lua_getfield(L, -1, "__index");
		lua_setfield(L, -3, "MONITOR_METHODS");
		lua_pop(L, 1);

		set_func(L, monitor_new, "monitor");
	}

	set_func_if_symbol_exists("sd_pid_get_session", L, pid_get_session, "pid_get_session");
	set_func_if_symbol_exists("sd_pid_get_unit", L, pid_get_unit, "pid_get_unit");
	set_func_if_symbol_exists("sd_pid_get_user_unit", L, pid_get_user_unit, "pid_get_user_unit");
	set_func_if_symbol_exists("sd_pid_get_owner_uid", L, pid_get_owner_uid, "pid_get_owner_uid");
	set_func_if_symbol_exists("sd_pid_get_machine_name", L, pid_get_machine_name, "pid_get_machine_name");
	set_func_if_symbol_exists("sd_pid_get_slice", L, pid_get_slice, "pid_get_slice");

	set_func_if_symbol_exists("sd_uid_get_state", L, uid_get_state, "uid_get_state");
	set_func_if_symbol_exists("sd_uid_is_on_seat", L, uid_is_on_seat, "uid_is_on_seat");
	set_func_if_symbol_exists("sd_uid_get_sessions", L, uid_get_sessions, "uid_get_sessions");
	set_func_if_symbol_exists("sd_uid_get_seats", L, uid_get_seats, "uid_get_seats");

	set_func_if_symbol_exists("sd_session_is_active", L, session_is_active, "session_is_active");
	set_func_if_symbol_exists("sd_session_is_remote", L, session_is_remote, "session_is_remote");
	set_func_if_symbol_exists("sd_session_get_state", L, session_get_state, "session_get_state");
	set_func_if_symbol_exists("sd_session_get_uid", L, session_get_uid, "session_get_uid");
	set_func_if_symbol_exists("sd_session_get_seat", L, session_get_seat, "session_get_seat");
	set_func_if_symbol_exists("sd_session_get_service", L, session_get_service, "session_get_service");
	set_func_if_symbol_exists("sd_session_get_type", L, session_get_type, "session_get_type");
	set_func_if_symbol_exists("sd_session_get_class", L, session_get_class, "session_get_class");
	set_func_if_symbol_exists("sd_session_get_display", L, session_get_display, "session_get_display");
	set_func_if_symbol_exists("sd_session_get_remote_host", L, session_get_remote_host, "session_get_remote_host");
	set_func_if_symbol_exists("sd_session_get_remote_user", L, session_get_remote_user, "session_get_remote_user");
	set_func_if_symbol_exists("sd_session_get_tty", L, session_get_tty, "session_get_tty");
	set_func_if_symbol_exists("sd_session_get_vt", L, session_get_vt, "session_get_vt");

	set_func_if_symbol_exists("sd_seat_get_active", L, seat_get_active, "seat_get_active");
	set_func_if_symbol_exists("sd_seat_get_sessions", L, seat_get_sessions, "seat_get_sessions");
	set_func_if_symbol_exists("sd_seat_can_multi_session", L, seat_can_multi_session, "seat_can_multi_session");
	set_func_if_symbol_exists("sd_seat_can_tty", L, seat_can_tty, "seat_can_tty");
	set_func_if_symbol_exists("sd_seat_can_graphical", L, seat_can_graphical, "seat_can_graphical");

	set_func_if_symbol_exists("sd_get_seats", L, get_seats, "get_seats");
	set_func_if_symbol_exists("sd_get_sessions", L, get_sessions, "get_sessions");
	set_func_if_symbol_exists("sd_get_uids", L, get_uids, "get_uids");
	set_func_if_symbol_exists("sd_get_machine_names", L, get_machine_names, "get_machine_names");
	/* 211 */
	set_func_if_symbol_exists("sd_machine_get_class", L, machine_get_class, "machine_get_class");
	set_func_if_symbol_exists("sd_peer_get_session", L, peer_get_session, "peer_get_session");
	set_func_if_symbol_exists("sd_peer_get_owner_uid", L, peer_get_owner_uid, "peer_get_owner_uid");
	set_func_if_symbol_exists("sd_peer_get_unit", L, peer_get_unit, "peer_get_unit");
	set_func_if_symbol_exists("sd_peer_get_user_unit", L, peer_get_user_unit, "peer_get_user_unit");
	set_func_if_symbol_exists("sd_peer_get_machine_name", L, peer_get_machine_name, "peer_get_machine_name");
	set_func_if_symbol_exists("sd_peer_get_slice", L, peer_get_slice, "peer_get_slice");
	/* 213 */
	set_func_if_symbol_exists("sd_sd_uid_get_display", L, uid_get_display, "sd_uid_get_display");
	/* 216 */
	set_func_if_symbol_exists("sd_machine_get_ifindices", L, machine_get_ifindices, "machine_get_ifindices");
	/* 217 */
	set_func_if_symbol_exists("sd_session_get_desktop", L, session_get_desktop, "session_get_desktop");

	return 1;
}
