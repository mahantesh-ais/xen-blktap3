pyright (C) 2012      Advanced Micro Devices
 * Author Christoph Egger <Christoph.Egger@amd.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; version 2.1 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 */

#include "libxl_osdeps.h" /* must come before any other headers */
#include "libxl_internal.h"

#include "tap-ctl.h"
#include "blktap.h"

static int blktap_find(const char *type, const char *path, struct tap_list *tap)
{
    struct tqh_tap_list list;
    struct tap_list *entry, *next_t;
    int ret = -ENOENT, err;

    TAILQ_INIT(&list);
    if ((err = tap_ctl_list(&list)))
        return err;

    if (TAILQ_EMPTY(&list))
        return ret;

    tap_list_for_each_entry_safe(entry, next_t, &list) {

        if (type && (!entry->type || strcmp(entry->type, type)))
            continue;

        if (path && (!entry->path || strcmp(entry->path, path)))
            continue;

        *tap = *entry;
        tap->type = tap->path = NULL;
        ret = 0;
        break;
    }

    tap_ctl_list_free(&list);

    return ret;
}

/**
 * blktap3 doesn't require blkback, so it's always available.
 */
int libxl__blktap_enabled(libxl__gc *gc)
{
    return 1;
}

int libxl__blktap_devpath(libxl__gc *gc, const char *disk,
		libxl_disk_format format)
{
    const char *type = NULL;
    char *params = NULL;
    struct tap_list tap;
    int err = 0;

    type = libxl__device_disk_string_of_format(format);
    if (!(err = blktap_find(type, disk, &tap))) {
        LOG(DEBUG, "found tapdisk\n");
		return 0;
    }

    LOG(DEBUG, "tapdisk not found\n");

	/* TODO Should we worry about return codes other than ENOENT? */

    params = libxl__sprintf(gc, "%s:%s", type, disk);
    if (!(err = tap_ctl_create(params, 0, -1, NULL))) {
        LOG(DEBUG, "created tapdisk\n");		
        return 0;
    }

    LOG(ERROR, "error creating tapdisk: %s\n", strerror(err));

    return err;
}

int libxl__device_destroy_tapdisk(libxl__gc *gc, const char *be_path)
{
    char *disk;
    int err;
    struct tap_list tap;

	LOG(DEBUG, "destroying tapdisk %s\n", be_path);

    disk = strchr(be_path, ':');
    if (!disk) {
        LOG(ERROR, "Unable to parse params %s", be_path);
        return ERROR_INVAL;
    }

    *disk++ = '\0';

    err = blktap_find(be_path, disk, &tap);
    if (err < 0) {
        /* returns -errno */
        LOGEV(ERROR, -err, "Unable to find type %s disk %s", be_path, disk);
        return ERROR_FAIL;
    }

    err = tap_ctl_destroy(tap.pid, tap.minor, 0, NULL);
    if (err < 0) {
        LOGEV(ERROR, -err, "Failed to destroy tap device id %d minor %d",
              tap.pid, tap.minor);
        return ERROR_FAIL;
    }

    return 0;
}

/*
 * Local variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
