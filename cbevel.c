/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2022 Beaver (GEGL custom bevel)
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

enum_start (gegl_blend_mode_type)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT, "Hardlight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "ColorDodge",
              N_("Color Dodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUS,      "Plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN,      "Darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN,      "Lighten",
              N_("Lighten"))
enum_end (GeglBlendModeType)

property_enum (blendmode, _("Blend Mode of internal Emboss"),
    GeglBlendModeType, gegl_blend_mode_type,
    GEGL_BLEND_MODE_TYPE_HARDLIGHT)




enum_start (gegl_median_blur_neighborhoodcb)
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_SQUAREcb,  "squarecb",  N_("Square"))
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_CIRCLEcb,  "circlecb",  N_("Circle"))
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_DIAMONDcb, "diamondcb", N_("Diamond"))
enum_end (GeglMedianBlurNeighborhoodcb)


property_enum (type, _("Choose Internal Median Shape"),
               GeglMedianBlurNeighborhoodcb, gegl_median_blur_neighborhoodcb,
               GEGL_MEDIAN_BLUR_NEIGHBORHOOD_CIRCLEcb)
  description (_("Neighborhood type"))




property_double (opacity, _("Make wider (above 2 will harm dropshadow in a graph)"), 4)
    description (_("Global opacity value that is always used on top of the optional auxiliary input buffer."))
    value_range (1.0, 4.0)
    ui_range    (1.0, 4.0)



property_double (azimuth, _("Azimuth"), 67.0)
    description (_("Light angle (degrees)"))
    value_range (30, 90)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (elevation, _("Elevation"), 25.0)
    description (_("Elevation angle (degrees)"))
    value_range (25, 90)
    ui_meta ("unit", "degree")

property_int (depth, _("Depth (makes darker)"), 24)
    description (_("Filter width"))
    value_range (1, 70)



property_int  (size, _("Internal Median Blur Radius"), -5)
  value_range (-10, 15)
  ui_range    (-10, 15)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))

property_double  (percentile, _("Internal Median Blur Percentile"), -53)
  value_range (20, 80)
  description (_("Neighborhood color percentile"))
    ui_meta     ("role", "output-extent")

property_double  (alpha_percentile, _("Internal Median Blur Alpha percentile"), -68)
  value_range (-90, 100)
  description (_("Neighborhood alpha percentile"))



property_double (gaus, _("Internal Gaussian Blur"), 1)
   description (_("Standard deviation for the XY axis"))
   value_range (0.0, 9.0)

property_int (box, _("Internal Box Blur"), 8)
   description(_("Radius of square pixel region, (width and height will be radius*2+1)"))
   value_range (0, 13)
   ui_range    (0, 13)
   ui_gamma   (1.5)




property_int  (mcb, _("Smooth bevel"), 1)
  description (_("Controls the number of iterations"))
  value_range (0, 10)
  ui_range    (0, 10)

property_double (sharpen, _("Sharpen"), 1)
    description(_("Scaling factor for unsharp-mask, the strength of effect"))
    value_range (0.0, 4.5)
    ui_range    (0.0, 4.5)
    ui_gamma    (3.0)


property_file_path(src, _("Image file Overlay - Desaturate and ligthen for best results"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))




property_double (desat, _("Desaturate for image file overlay"), 1.0)
    description(_("Scale, strength of effect"))
    value_range (0.0, 1.3)
    ui_range (0.0, 1.3)


property_double (lightness, _("Lightness that can help image file overlay brighten"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-2, 18.0)

property_double (hue, _("Hue Rotation"),  0.0)
   description  (_("Hue adjustment"))
   value_range  (-180.0, 180.0)




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     cbevel
#define GEGL_OP_C_SOURCE cbevel.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *median;
  GeglNode *box;
  GeglNode *gaussian;
  GeglNode *hardlight;
  GeglNode *multiply;
  GeglNode *colordodge;
  GeglNode *emboss;
  GeglNode *plus;
  GeglNode *darken;
  GeglNode *lighten;
  GeglNode *opacity;
  GeglNode *mcb;
  GeglNode *sharpen;
  GeglNode *desat;
  GeglNode *multiply2;
  GeglNode *imagefileoverlay;
  GeglNode *lightness;
  GeglNode *output;
}State;

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  GeglNode *usethis = state->hardlight; /* the default */
  switch (o->blendmode) {
    case GEGL_BLEND_MODE_TYPE_MULTIPLY: usethis = state->multiply; break;
    case GEGL_BLEND_MODE_TYPE_COLORDODGE: usethis = state->colordodge; break;
    case GEGL_BLEND_MODE_TYPE_PLUS: usethis = state->plus; break;
    case GEGL_BLEND_MODE_TYPE_DARKEN: usethis = state->darken; break;
    case GEGL_BLEND_MODE_TYPE_LIGHTEN: usethis = state->lighten; break;
  }
  gegl_node_link_many (state->input, state->median, state->box, state->gaussian, usethis, state->opacity, state->mcb, state->sharpen, state->desat, state->multiply2, state->lightness, state->output,  NULL);
  gegl_node_connect_from (usethis, "aux", state->emboss, "output");

}

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *median, *multiply, *hardlight, *colordodge, *darken, *desat, *multiply2, *lighten, *plus, *opacity, *gaussian, *emboss, *box, *lightness, *imagefileoverlay, *mcb, *sharpen;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");



  median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);


  multiply    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  multiply2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  hardlight    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hard-light",
                                  NULL);

  colordodge    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-dodge",
                                  NULL);

  darken    = gegl_node_new_child (gegl,
                                  "operation", "gegl:darken",
                                  NULL);

  lighten    = gegl_node_new_child (gegl,
                                  "operation", "gegl:lighten",
                                  NULL);

  plus    = gegl_node_new_child (gegl,
                                  "operation", "gegl:plus",
                                  NULL);



  opacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);


  gaussian    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);


  emboss    = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

  box    = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);

  imagefileoverlay    = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  lightness    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  mcb    = gegl_node_new_child (gegl,
                                  "operation", "gegl:mean-curvature-blur",
                                  NULL);

  sharpen    = gegl_node_new_child (gegl,
                                  "operation", "gegl:unsharp-mask",
                                  NULL);

  desat   = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);


 
 
  gegl_operation_meta_redirect (operation, "size", median, "radius");
  gegl_operation_meta_redirect (operation, "gaus", gaussian, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", gaussian, "std-dev-y");
  gegl_operation_meta_redirect (operation, "azimuth", emboss, "azimuth");
  gegl_operation_meta_redirect (operation, "elevation", emboss, "elevation");
  gegl_operation_meta_redirect (operation, "depth", emboss, "depth");
  gegl_operation_meta_redirect (operation, "percentile", median, "percentile");
  gegl_operation_meta_redirect (operation, "alpha-percentile", median, "alpha-percentile");
  gegl_operation_meta_redirect (operation, "src", imagefileoverlay, "src");
  gegl_operation_meta_redirect (operation, "lightness", lightness, "lightness");
  gegl_operation_meta_redirect (operation, "hue", lightness, "hue");
  gegl_operation_meta_redirect (operation, "opacity", opacity, "value");
  gegl_operation_meta_redirect (operation, "mcb", mcb, "iterations");
  gegl_operation_meta_redirect (operation, "sharpen", sharpen, "scale");
  gegl_operation_meta_redirect (operation, "box", box, "radius");
  gegl_operation_meta_redirect (operation, "type", median, "neighborhood");
  gegl_operation_meta_redirect (operation, "desat", desat, "scale");




  gegl_node_link_many (input, median, box, gaussian, hardlight, opacity, mcb, sharpen, desat, multiply2, lightness, output,  NULL);
  gegl_node_connect_from (hardlight, "aux", emboss, "output");
  gegl_node_connect_from (multiply2, "aux", imagefileoverlay, "output");
  gegl_node_link_many (gaussian, emboss,  NULL);
  gegl_node_link_many (imagefileoverlay,  NULL);

  /* now save references to the gegl nodes so we can use them
   * later, when update_graph() is called
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->median = median;
  state->box = box;
  state->gaussian = gaussian;
  state->hardlight = hardlight;
  state->multiply = multiply;
  state->colordodge = colordodge;
  state->emboss = emboss;
  state->plus = plus;
  state->darken = darken;
  state->lighten = lighten;
  state->opacity = opacity;
  state->mcb = mcb;
  state->sharpen = sharpen;
  state->desat = desat;
  state->multiply2 = multiply2;
  state->imagefileoverlay = imagefileoverlay;
  state->lightness = lightness;
  state->output = output;

  o->user_data = state;
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:custom-bevel",
    "title",       _("Custom Bevel"),
    "categories",  "Artistic",
    "reference-hash", "h3do6akv00vyeefjf25sb2ac",
    "description", _("A highly customizable bevel that lets you change internal blend modes, median shape types and internal blurs"
                     ""),
    NULL);
}

#endif
