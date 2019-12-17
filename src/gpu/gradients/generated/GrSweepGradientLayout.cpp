/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/**************************************************************************************************
 *** This file was autogenerated from GrSweepGradientLayout.fp; do not modify.
 **************************************************************************************************/
#include "GrSweepGradientLayout.h"

#include "include/gpu/GrTexture.h"
#include "src/gpu/glsl/GrGLSLFragmentProcessor.h"
#include "src/gpu/glsl/GrGLSLFragmentShaderBuilder.h"
#include "src/gpu/glsl/GrGLSLProgramBuilder.h"
#include "src/sksl/SkSLCPP.h"
#include "src/sksl/SkSLUtil.h"
class GrGLSLSweepGradientLayout : public GrGLSLFragmentProcessor {
public:
    GrGLSLSweepGradientLayout() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrSweepGradientLayout& _outer = args.fFp.cast<GrSweepGradientLayout>();
        (void)_outer;
        auto gradientMatrix = _outer.gradientMatrix;
        (void)gradientMatrix;
        auto bias = _outer.bias;
        (void)bias;
        auto scale = _outer.scale;
        (void)scale;
        biasVar = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf_GrSLType, "bias");
        scaleVar =
                args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf_GrSLType, "scale");
        SkString sk_TransformedCoords2D_0 =
                fragBuilder->ensureCoords2D(args.fTransformedCoords[0].fVaryingPoint);
        fragBuilder->codeAppendf(
                "half angle;\nif (sk_Caps.atan2ImplementedAsAtanYOverX) {\n    angle = half(2.0 * "
                "atan(-%s.y, length(%s) - %s.x));\n} else {\n    angle = half(atan(-%s.y, "
                "-%s.x));\n}\nhalf t = ((angle * 0.15915493667125702 + 0.5) + %s) * %s;\n%s = "
                "half4(t, 1.0, 0.0, 0.0);\n",
                sk_TransformedCoords2D_0.c_str(), sk_TransformedCoords2D_0.c_str(),
                sk_TransformedCoords2D_0.c_str(), sk_TransformedCoords2D_0.c_str(),
                sk_TransformedCoords2D_0.c_str(), args.fUniformHandler->getUniformCStr(biasVar),
                args.fUniformHandler->getUniformCStr(scaleVar), args.fOutputColor);
    }

private:
    void onSetData(const GrGLSLProgramDataManager& pdman,
                   const GrFragmentProcessor& _proc) override {
        const GrSweepGradientLayout& _outer = _proc.cast<GrSweepGradientLayout>();
        {
            float biasValue = _outer.bias;
            if (biasPrev != biasValue) {
                biasPrev = biasValue;
                pdman.set1f(biasVar, biasValue);
            }
            float scaleValue = _outer.scale;
            if (scalePrev != scaleValue) {
                scalePrev = scaleValue;
                pdman.set1f(scaleVar, scaleValue);
            }
        }
    }
    float biasPrev = SK_FloatNaN;
    float scalePrev = SK_FloatNaN;
    UniformHandle biasVar;
    UniformHandle scaleVar;
};
GrGLSLFragmentProcessor* GrSweepGradientLayout::onCreateGLSLInstance() const {
    return new GrGLSLSweepGradientLayout();
}
void GrSweepGradientLayout::onGetGLSLProcessorKey(const GrShaderCaps& caps,
                                                  GrProcessorKeyBuilder* b) const {}
bool GrSweepGradientLayout::onIsEqual(const GrFragmentProcessor& other) const {
    const GrSweepGradientLayout& that = other.cast<GrSweepGradientLayout>();
    (void)that;
    if (gradientMatrix != that.gradientMatrix) return false;
    if (bias != that.bias) return false;
    if (scale != that.scale) return false;
    return true;
}
GrSweepGradientLayout::GrSweepGradientLayout(const GrSweepGradientLayout& src)
        : INHERITED(kGrSweepGradientLayout_ClassID, src.optimizationFlags())
        , fCoordTransform0(src.fCoordTransform0)
        , gradientMatrix(src.gradientMatrix)
        , bias(src.bias)
        , scale(src.scale) {
    this->addCoordTransform(&fCoordTransform0);
}
std::unique_ptr<GrFragmentProcessor> GrSweepGradientLayout::clone() const {
    return std::unique_ptr<GrFragmentProcessor>(new GrSweepGradientLayout(*this));
}
GR_DEFINE_FRAGMENT_PROCESSOR_TEST(GrSweepGradientLayout);
#if GR_TEST_UTILS
std::unique_ptr<GrFragmentProcessor> GrSweepGradientLayout::TestCreate(GrProcessorTestData* d) {
    SkScalar scale = GrGradientShader::RandomParams::kGradientScale;
    SkPoint center = {d->fRandom->nextRangeScalar(0.0f, scale),
                      d->fRandom->nextRangeScalar(0.0f, scale)};

    GrGradientShader::RandomParams params(d->fRandom);
    auto shader = params.fUseColors4f
                          ? SkGradientShader::MakeSweep(center.fX, center.fY, params.fColors4f,
                                                        params.fColorSpace, params.fStops,
                                                        params.fColorCount)
                          : SkGradientShader::MakeSweep(center.fX, center.fY, params.fColors,
                                                        params.fStops, params.fColorCount);
    GrTest::TestAsFPArgs asFPArgs(d);
    std::unique_ptr<GrFragmentProcessor> fp = as_SB(shader)->asFragmentProcessor(asFPArgs.args());
    SkASSERT_RELEASE(fp);
    return fp;
}
#endif

std::unique_ptr<GrFragmentProcessor> GrSweepGradientLayout::Make(const SkSweepGradient& grad,
                                                                 const GrFPArgs& args) {
    SkMatrix matrix;
    if (!grad.totalLocalMatrix(args.fPreLocalMatrix, args.fPostLocalMatrix)->invert(&matrix)) {
        return nullptr;
    }
    matrix.postConcat(grad.getGradientMatrix());
    return std::unique_ptr<GrFragmentProcessor>(
            new GrSweepGradientLayout(matrix, grad.getTBias(), grad.getTScale()));
}
