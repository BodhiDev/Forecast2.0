#include <e.h>
#include "e_mod_main.h"


struct _E_Config_Dialog_Data
{
   double poll_time;
   double days;
   int    display;
   int    degrees;
   char  *code;
   char  *lang;
   char  *label;
   int    show_text;
   int    popup_on_hover;
};

static void        *_create_data(E_Config_Dialog *cfd);
static void         _free_data(E_Config_Dialog *cfd __UNUSED__, E_Config_Dialog_Data *cfdata);
static Evas_Object *_basic_create_widgets(E_Config_Dialog *cfd __UNUSED__, Evas *evas,
                                          E_Config_Dialog_Data *cfdata);
static int          _basic_apply_data(E_Config_Dialog *cfd,
                                      E_Config_Dialog_Data *cfdata);
static void         _fill_data(Config_Item *ci, E_Config_Dialog_Data *cfdata);

void
_config_forecasts_module(Config_Item *ci)
{
   E_Config_Dialog *cfd;
   E_Config_Dialog_View *v;
   E_Container *con;
   // char buf[4096];

   v = E_NEW(E_Config_Dialog_View, 1);

   v->create_cfdata = _create_data;
   v->free_cfdata = _free_data;
   v->basic.apply_cfdata = _basic_apply_data;
   v->basic.create_widgets = _basic_create_widgets;
   v->override_auto_apply = 1;

   // Icon for setting dialog
   //~ snprintf(buf, sizeof(buf), "%s/module.edj",
            //~ e_module_dir_get(forecasts_config->module));
   con = e_container_current_get(e_manager_current_get());
   cfd = e_config_dialog_new(con, D_("Forecasts Settings"), "Forecasts", 
         "_e_modules_forecasts_config_dialog", "preferences-system", 0, v, ci);
   forecasts_config->config_dialog = cfd;
}

static void
_fill_data(Config_Item *ci, E_Config_Dialog_Data *cfdata)
{
   cfdata->poll_time = (ci->poll_time / 60.0);
   cfdata->days = ci->days;
   cfdata->degrees = ci->degrees;
   if (ci->code)
     cfdata->code = strdup(ci->code);
   if (ci->lang)
     cfdata->lang = strdup(ci->lang);
   if (ci->label)
     cfdata->label = strdup(ci->label);
   cfdata->show_text = ci->show_text;
   cfdata->popup_on_hover = ci->popup_on_hover;
}


static void *
_create_data(E_Config_Dialog *cfd)
{
   E_Config_Dialog_Data *cfdata;
   Config_Item *ci;

   ci = cfd->data;
   cfdata = E_NEW(E_Config_Dialog_Data, 1);

   _fill_data(ci, cfdata);
   return cfdata;
}

static void
_free_data(E_Config_Dialog *cfd __UNUSED__, E_Config_Dialog_Data *cfdata)
{
   if (!forecasts_config)
     return;

   forecasts_config->config_dialog = NULL;
   free(cfdata);
   cfdata = NULL;
}

static Evas_Object *
_basic_create_widgets(E_Config_Dialog *cfd __UNUSED__, Evas *evas,
                      E_Config_Dialog_Data *cfdata)
{
   Evas_Object *o, *of, *ob;
   E_Radio_Group *dg;

   o = e_widget_list_add(evas, 0, 0);
   of = e_widget_framelist_add(evas, D_("Display Settings"), 0);
   ob = e_widget_label_add(evas, D_("Poll Time"));
   e_widget_framelist_object_append(of, ob);
   ob = e_widget_slider_add(evas, 1, 0, D_("%2.0f minutes"), 15.0, 60.0, 1.0, 0,&(cfdata->poll_time), NULL, 40);
   e_widget_framelist_object_append(of, ob);

   ob = e_widget_label_add(evas, D_("Forecasts days"));
   e_widget_framelist_object_append(of, ob);
   ob = e_widget_slider_add(evas, 1, 0, D_("%2.0f days"), 2.0, 3.0, 1.0, 0, &(cfdata->days), NULL, 40);
   e_widget_framelist_object_append(of, ob);
   
   ob = e_widget_check_add(evas, D_("Show Description"), &(cfdata->show_text));
   e_widget_framelist_object_append(of, ob);
   ob = e_widget_check_add(evas, D_("Popup on mouse over"), &(cfdata->popup_on_hover));
   e_widget_framelist_object_append(of, ob);
   e_widget_list_object_append(o, of, 1, 1, 0.5);

   of = e_widget_framelist_add(evas, D_("Unit Settings"), 0);
   dg = e_widget_radio_group_new(&(cfdata->degrees));
   ob = e_widget_radio_add(evas, D_("Metric"), DEGREES_C, dg);
   e_widget_framelist_object_append(of, ob);
   ob = e_widget_radio_add(evas, D_("English"), DEGREES_F, dg);
   e_widget_framelist_object_append(of, ob);
   e_widget_list_object_append(o, of, 1, 1, 0.5);

   of = e_widget_frametable_add(evas, D_("Weather location"), 0);

   ob = e_widget_label_add(evas, D_("City name (empty = local) "));
   e_widget_frametable_object_append(of, ob, 0, 1, 1, 1, 1, 0, 1, 0);
   ob = e_widget_entry_add(evas, &cfdata->code, NULL, NULL, NULL);
   e_widget_size_min_set(ob, 120, 28);
   e_widget_frametable_object_append(of, ob, 1, 1, 1, 1, 1, 0, 1, 0);

   ob = e_widget_label_add(evas, D_("Custom location label "));
   e_widget_frametable_object_append(of, ob, 0, 2, 1, 1, 1, 0, 1, 0);
   ob = e_widget_entry_add(evas, &cfdata->label, NULL, NULL, NULL);
   e_widget_size_min_set(ob, 120, 28);
   e_widget_frametable_object_append(of, ob, 1, 2, 1, 1, 1, 0, 1, 0);

   e_widget_list_object_append(o, of, 1, 1, 0.5);

   of = e_widget_frametable_add(evas, D_("Language support"), 0);

   ob = e_widget_label_add(evas, D_("Language code (e.g. sk)    "));
   e_widget_frametable_object_append(of, ob, 0, 1, 1, 1, 1, 0, 1, 0);
   ob = e_widget_entry_add(evas, &cfdata->lang, NULL, NULL, NULL);
   e_widget_size_min_set(ob, 40, 28);
   e_widget_frametable_object_append(of, ob, 1, 1, 1, 1, 1, 0, 1, 0);

   e_widget_list_object_append(o, of, 1, 1, 0.5);

   return o;
}

static int
_basic_apply_data(E_Config_Dialog *cfd, E_Config_Dialog_Data *cfdata)
{
   Config_Item *ci;
   char *t = "", *l = "", *k = "";

   ci = cfd->data;

   ci->degrees = cfdata->degrees;
   ci->poll_time = (cfdata->poll_time * 60.0);
   ci->days = cfdata->days;

   if (ci->code)
     eina_stringshare_del(ci->code);

   if (cfdata->code)
     t = strdup(cfdata->code);
   *t = toupper(*t);
   ci->code = eina_stringshare_add(t);

   if (cfdata->lang)
     l = strdup(cfdata->lang);
   *l = tolower(*l);
   ci->lang = eina_stringshare_add(l);

   if (cfdata->label)
     k = strdup(cfdata->label);
   ci->label = eina_stringshare_add(k);

   free(t);
   free(l);
   free(k);
   ci->show_text = cfdata->show_text;
   ci->popup_on_hover = cfdata->popup_on_hover;

   e_config_save_queue();
   _fc_config_updated(ci);
   return 1;
}

