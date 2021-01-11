/*
 * ite-epdc.c
 *
 *  Created on: 04.01.2021
 *      Author: oliver.lenz
 */

#include "ite/ite-it8951.h"
#include <ite/ite-epdc.h>
#include <pl/types.h>
#include <pl/epdc.h>
#include <pl/gpio.h>
#include <stdlib.h>
#include "assert.h"

#define LOG_TAG "ite-epdc"
#include "utils.h"

I80IT8951DevInfo pBuf;

static int ite_epdc_clear_init(struct pl_epdc *epdc)
{
    struct it8951 *p = epdc->data;

    return it8951_clear_init(p);
}

static int ite_epdc_update(struct pl_epdc *epdc, int wfid,
                           enum pl_update_mode mode, const struct pl_area *area)
{
    struct it8951 *p = epdc->data;

    return it8951_update(p, wfid, mode, area);
}

static int ite_epdc_wait_update_end(struct pl_epdc *epdc)
{
    struct it8951 *p = epdc->data;

    return it8951_wait_update_end(p);
}

static int ite_epdc_set_power(struct pl_epdc *epdc,
                              enum pl_epdc_power_state state)
{
    struct it8951 *p = epdc->data;

    if (it8951_set_power_state(p, state))
        return -1;

    epdc->power_state = state;

    return 0;
}

static int ite_epdc_set_epd_power(struct pl_epdc *epdc, int on)
{
    struct it8951 *p = epdc->data;

    return it8951_set_epd_power(p, on);
}

int ite_epdc_init(struct pl_epdc *epdc, const struct pl_dispinfo *dispinfo,
                  struct it8951 *it8951)
{

    int stat;
    stat = 0;

    assert(epdc != NULL);
    assert(dispinfo != NULL);
    assert(it8951 != NULL);
    assert(it8951->data != NULL);

    if (it8951->data->hrdy != PL_GPIO_NONE)
        LOG("Using HRDY GPIO");

    it8951->flags.needs_update = 0;

    epdc->clear_init = ite_epdc_clear_init;
    epdc->update = ite_epdc_update;
    epdc->wait_update_end = ite_epdc_wait_update_end;
    epdc->set_power = ite_epdc_set_power;
    epdc->set_epd_power = ite_epdc_set_epd_power;
    epdc->data = it8951;
    epdc->dispinfo = dispinfo;

    if (stat)
        return -1;

    it8951_load_init_code(it8951, &pBuf);

    it8951->xres = epdc->xres;
    it8951->yres = epdc->yres;

    LOG("Ready %dx%d", epdc->xres, epdc->yres);

    return 0;
    return 0;
}
