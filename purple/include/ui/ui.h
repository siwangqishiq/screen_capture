#pragma once

#include <string>
#include <memory>
#include <vector>

namespace purple{
    const int LAYOUT_MATCH_PARENT = -1;
    const int LAYOUT_WRAP_CONTENT = -2;
    const int LAYOUT_UNSET = -3;

    //控件最大宽高
    const int WIDGET_MAX_WIDTH = 100000000;
    const int WIDGET_MAX_HEIGHT = 100000000;

    //测量模式
    enum MeasureSpecMode{
        Exactly = 1, //准确数值
        Atmost = 2, //最大数值
        Unset = 0 //未设置 后面会补上
    };

    class Widget;
    class Container;

    typedef std::shared_ptr<Widget> PWidget;
    typedef std::shared_ptr<Container> PContainer;

    enum VisibleState{
        Normal = 0,
        InVisible = -1,
        Gone = -2
    };

    PWidget FunContainer(std::vector<PWidget> children);
}
