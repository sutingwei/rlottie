
// canvas renderer backend

#pragma once

namespace rlottie {

namespace canvas {

struct Color {
    float r, g, b, a;
};

struct Paint {
    float xform[6];
    float extent[2];
    float radius;
    float feather;
    Color innerColor;
    Color outerColor;
    int   image;
};

struct GlyphPosition {
    const char* str;         // Position of the glyph in the input string.
    float       x;           // The x-coordinate of the logical glyph position.
    float       minX, maxX;  // The bounds of the glyph shape.
};

struct TextRow {
    const char* start;  // Pointer to the input text where the row starts.
    const char* end;   // Pointer to the input text where the row ends (one past
                       // the last character).
    const char* next;  // Pointer to the beginning of the next row.
    float       width;  // Logical width of the row.
    float       minX,
        maxX;  // Actual bounds of the row. Logical with and bounds can differ
               // because of kerning and some parts over extending.
};

struct CanvasRenderer {
    // Begin drawing a new frame
    void (*beginFrame)(void* ctx, float windowWidth, float windowHeight,
                       float devicePixelRatio);
    void (*cancelFrame)(void* ctx);
    void (*endFrame)(void* ctx);

    // Composite operation
    void (*globalCompositeOperation)(void* ctx, int op);
    void (*globalCompositeBlendFunc)(void* ctx, int sFactor, int dFactor);
    void (*globalCompositeBlendFuncSeparate)(void* ctx, int srcRGB, int dstRGB,
                                             int srcAlpha, int dstAlpha);

    // Color utils
    Color (*rgb)(unsigned char r, unsigned char g, unsigned char b);
    Color (*rgbF)(float r, float g, float b);
    Color (*rgba)(unsigned char r, unsigned char g, unsigned char b,
                  unsigned char a);
    Color (*rgbaF)(float r, float g, float b, float a);
    Color (*lerpRgba)(Color c0, Color c1, float u);
    Color (*transRgba)(Color c0, unsigned char a);
    Color (*transRgbaF)(Color c0, float a);
    Color (*hsl)(float h, float s, float l);
    Color (*hsla)(float h, float s, float l, unsigned char a);

    // State Handling
    void (*save)(void* ctx);
    void (*restore)(void* ctx);
    void (*reset)(void* ctx);

    // Render styles
    void (*shapeAntiAlias)(void* ctx, int enabled);
    void (*strokeColor)(void* ctx, Color color);
    void (*strokePaint)(void* ctx, Paint paint);
    void (*fillColor)(void* ctx, Color color);
    void (*fillPaint)(void* ctx, Paint paint);
    void (*miterLimit)(void* ctx, float limit);
    void (*strokeWidth)(void* ctx, float size);
    void (*lineCap)(void* ctx, int cap);
    void (*lineJoin)(void* ctx, int join);
    void (*globalAlpha)(void* ctx, float alpha);

    // Transforms
    void (*resetTransform)(void* ctx);
    void (*transform)(void* ctx, float a, float b, float c, float d, float e,
                      float f);
    void (*translate)(void* ctx, float x, float y);
    void (*rotate)(void* ctx, float angle);
    void (*skewX)(void* ctx, float angle);
    void (*skewY)(void* ctx, float angle);
    void (*scale)(void* ctx, float x, float y);
    void (*currentTransform)(void* ctx, float* xform);
    void (*transformIdentity)(float* dst);
    void (*transformTranslate)(float* dst, float tx, float ty);
    void (*transformScale)(float* dst, float sx, float sy);
    void (*transformRotate)(float* dst, float a);
    void (*transformSkewX)(float* dst, float a);
    void (*transformSkewY)(float* dst, float a);
    void (*transformMultiply)(float* dst, const float* src);
    void (*transformPremultiply)(float* dst, const float* src);
    int (*transformInverse)(float* dst, const float* src);
    void (*transformPoint)(float* dstX, float* dstY, const float* xform,
                           float srcX, float srcY);
    float (*degToRad)(float deg);
    float (*radToDeg)(float rad);

    // Images
    int (*createImage)(void* ctx, const char* filename, int imageFlags);
    int (*createImageMem)(void* ctx, int imageFlags, unsigned char* data,
                          int nData);
    int (*createImageRGBA)(void* ctx, int w, int h, int imageFlags,
                           const unsigned char* data);
    void (*updateImage)(void* ctx, int image, const unsigned char* data);
    void (*imageSize)(void* ctx, int image, int* w, int* h);
    void (*deleteImage)(void* ctx, int image);

    // Paints
    Paint (*linearGradient)(void* ctx, float sx, float sy, float ex, float ey,
                            Color iCol, Color oCol);
    Paint (*boxGradient)(void* ctx, float x, float y, float w, float h, float r,
                         float f, Color iCol, Color oCol);
    Paint (*radialGradient)(void* ctx, float cx, float cy, float inR,
                            float outR, Color iCol, Color oCol);
    Paint (*imagePattern)(void* ctx, float ox, float oy, float ex, float ey,
                          float angle, int image, float alpha);

    // Scissoring
    void (*scissor)(void* ctx, float x, float y, float w, float h);
    void (*intersectScissor)(void* ctx, float x, float y, float w, float h);
    void (*resetScissor)(void* ctx);

    // Paths
    void (*beginPath)(void* ctx);
    void (*moveTo)(void* ctx, float x, float y);
    void (*lineTo)(void* ctx, float x, float y);
    void (*bezierTo)(void* ctx, float c1x, float c1y, float c2x, float c2y,
                     float x, float y);
    void (*quadTo)(void* ctx, float cx, float cy, float x, float y);
    void (*arcTo)(void* ctx, float x1, float y1, float x2, float y2,
                  float radius);
    void (*closePath)(void* ctx);
    void (*pathWinding)(void* ctx, int dir);
    void (*arc)(void* ctx, float cx, float cy, float r, float a0, float a1,
                int dir);
    void (*rect)(void* ctx, float x, float y, float w, float h);
    void (*roundedRect)(void* ctx, float x, float y, float w, float h, float r);
    void (*roundedRectVarying)(void* ctx, float x, float y, float w, float h,
                               float radTopLeft, float radTopRight,
                               float radBottomRight, float radBottomLeft);
    void (*ellipse)(void* ctx, float cx, float cy, float rx, float ry);
    void (*circle)(void* ctx, float cx, float cy, float r);
    void (*fill)(void* ctx);
    void (*stroke)(void* ctx);

    // Text
    int (*createFont)(void* ctx, const char* name, const char* filename);
    int (*createFontAtIndex)(void* ctx, const char* name, const char* filename,
                             const int fontIndex);
    int (*createFontMem)(void* ctx, const char* name, unsigned char* data,
                         int nData, int freeData);
    int (*createFontMemAtIndex)(void* ctx, const char* name,
                                unsigned char* data, int nData, int freeData,
                                const int fontIndex);
    int (*findFont)(void* ctx, const char* name);
    int (*addFallbackFontId)(void* ctx, int baseFont, int fallbackFont);
    int (*addFallbackFont)(void* ctx, const char* baseFont,
                           const char* fallbackFont);
    void (*fontSize)(void* ctx, float size);
    void (*fontBlur)(void* ctx, float blur);
    void (*textLetterSpacing)(void* ctx, float spacing);
    void (*textLineHeight)(void* ctx, float lineHeight);
    void (*textAlign)(void* ctx, int align);
    void (*fontFaceId)(void* ctx, int font);
    void (*fontFace)(void* ctx, const char* font);
    float (*text)(void* ctx, float x, float y, const char* string,
                  const char* end);
    void (*textBox)(void* ctx, float x, float y, float breakRowWidth,
                    const char* string, const char* end);
    float (*textBounds)(void* ctx, float x, float y, const char* string,
                        const char* end, float* bounds);
    void (*textBoxBounds)(void* ctx, float x, float y, float breakRowWidth,
                          const char* string, const char* end, float* bounds);
    int (*textGlyphPositions)(void* ctx, float x, float y, const char* string,
                              const char* end, GlyphPosition* positions,
                              int maxPositions);
    void (*textMetrics)(void* ctx, float* ascender, float* descender,
                        float* lineH);
    int (*textBreakLines)(void* ctx, const char* string, const char* end,
                          float breakRowWidth, TextRow* rows, int maxRows);
};

}  // namespace canvas

}  // namespace rlottie
