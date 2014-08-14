// mgline.cpp
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgline.h"
#include "mgshape_.h"

MG_IMPLEMENT_CREATE(MgLine)

MgLine::MgLine()
{
}

MgLine::~MgLine()
{
}

int MgLine::_getPointCount() const
{
    return 2;
}

Point2d MgLine::_getPoint(int index) const
{
    return _points[index ? 1 : 0];
}

void MgLine::_setPoint(int index, const Point2d& pt)
{
    _points[index ? 1 : 0] = pt;
}

int MgLine::_getHandleCount() const
{
    return 3;
}

Point2d MgLine::_getHandlePoint(int index) const
{
    return index < 2 ? _getPoint(index) : center();
}

bool MgLine::_setHandlePoint(int index, const Point2d& pt, float tol)
{
    return index < 2 && __super::_setHandlePoint(index, pt, tol);
}

bool MgLine::_isHandleFixed(int index) const
{
    return index >= 2;
}

int MgLine::_getHandleType(int index) const
{
    return index >= 2 ? kMgHandleMidPoint : __super::_getHandleType(index);
}

void MgLine::_copy(const MgLine& src)
{
    _points[0] = src._points[0];
    _points[1] = src._points[1];
    __super::_copy(src);
}

bool MgLine::_equals(const MgLine& src) const
{
    return (_points[0] == src._points[0]
            && _points[1] == src._points[1]
            && __super::_equals(src));
}

void MgLine::_update()
{
    _extent.set(_points[0], _points[1]);
    __super::_update();
}

void MgLine::_transform(const Matrix2d& mat)
{
    _points[0] *= mat;
    _points[1] *= mat;
    __super::_transform(mat);
}

void MgLine::_clear()
{
    _points[1] = _points[0];
    __super::_clear();
}

float MgLine::_hitTest(const Point2d& pt, float, MgHitResult& res) const
{
    return mglnrel::ptToLine(_points[0], _points[1], pt, res.nearpt);
}

bool MgLine::_hitTestBox(const Box2d& rect) const
{
    if (!__super::_hitTestBox(rect))
        return false;
    Point2d pts[2] = { _points[0], _points[1] };
    return mglnrel::clipLine(pts[0], pts[1], rect);
}

void MgLine::_output(MgPath& path) const
{
    path.moveTo(_points[0]);
    path.lineTo(_points[1]);
}

bool MgLine::_save(MgStorage* s) const
{
    bool ret = __super::_save(s);
    s->writeFloatArray("points", &(_points[0].x), 4);
    return ret;
}

bool MgLine::_load(MgShapeFactory* factory, MgStorage* s)
{
    bool ret = __super::_load(factory, s);
    return s->readFloatArray("points", &(_points[0].x), 4) == 4 && ret;
}