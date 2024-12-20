#pragma once

#include "log.h"
#include "render/common.h"
#include "ui/color.h"
#include "input/input_common.h"
#include "ui/ui.h"
#include <algorithm>
#include <functional>

namespace purple{
    class UiRoot;

    class Widget{
    public:
        Widget(){
            Log::i("widget","widget construct");
        }
        
        Widget(int w,int h):requestWidth_(w),requestHeight_(h){
            Log::i("widget","widget construct");
        }

        virtual ~Widget();

        void measure(MeasureSpecMode widthSpecMode, 
                        int widthValue, 
                        MeasureSpecMode heightSpecMode,
                        int heightValue);
        void layout(int l , int t);
        void render();

        virtual void onMeasure(MeasureSpecMode widthSpecMode,int widthValue, 
                                MeasureSpecMode heightSpecMode,int heightValue);
        virtual void onLayout(int l,int t);
        virtual void onRender();

        virtual int contentWidth();
        virtual int contentHeight();

        virtual bool dispatchInputEvent(InputEvent &event);

        virtual bool onInputEvent(InputEvent &event);

        void setParentWidget(Container *parent);

        int left = 0;
        int top = 0;

        std::string id;

        VisibleState getVisible(){
            return this->visible_;
        }

        void setVisible(VisibleState v){
            if(visible_ != v){
                visible_ = v;
            }
        }

        int getLeft(){
            return left;
        }

        int getTop(){
            return top;
        }

        int getWidth(){
            return width_;
        }

        int getHeight(){
            return height_;
        }

        int getContentHeight(){
            return getHeight() - paddingBottom_ - paddingTop_;
        }

        int getContentWidth(){
            return getWidth() - paddingLeft_ - paddingRight_;
        }

        int getMarginTop(){
            return marginTop_;
        }

        int getMarginLeft(){
            return marginLeft_;
        }

        int getMarginRight(){
            return marginRight_;
        }

        int getMarginHorizontal(){
            return marginLeft_ + marginRight_;
        }

        int getMarginVertical(){
            return marginTop_ + marginBottom_;
        }

        int getMarginBottom(){
            return marginBottom_;
        }

        int getPaddingHorizontal(){
            return paddingLeft_ + paddingRight_;
        }

        int getPaddingVertial(){
            return paddingTop_ + paddingBottom_;
        }

        LayoutGravity getLayoutGravity(){
            return this->layoutGravity_;
        }

        int getLayoutWeight(){
            return this->layoutWeight_;
        }

        void setWidth(int width){
            this->width_ = width;
        }

        void setHeight(int height){
            this->height_ = height;
        }

        void setRequestWidth(int reqW){
            this->requestWidth_ = reqW;
        }

        int getRequestWidth(){
            return this->requestWidth_;
        }

        void setRequestHeight(int reqH){
            this->requestHeight_ = reqH;
        }

        int getRequestHeight(){
            return this->requestHeight_;
        }

        Rect getWidgetRect();

        UiRoot* findRootUi();

        void setRootUi(UiRoot *root);

        void performClick(InputEvent &event);

        template<typename T>
        T& setPosition(int l,int t){
            left = l;
            top = t;
            return static_cast<T&>(*this);
        }

        template<typename T>
        T& setBackgroundColor(glm::vec4 color){
            this->bgColor_ = color;
            return static_cast<T&>(*this);
        }

        template<typename T>
        T& setSize(int requestW,int requestH){
            this->requestWidth_ = requestW;
            this->requestHeight_ = requestH;
            return static_cast<T&>(*this);
        }

        template<typename T>
        T& setPadding(int paddingLeft,int paddingTop,int paddingRight, int paddingBottom){
            this->paddingLeft_ = paddingLeft;
            this->paddingTop_ = paddingTop;
            this->paddingRight_ = paddingRight;
            this->paddingBottom_ = paddingBottom;
            return static_cast<T&>(*this);
        }

        template<typename T>
        T& setMargin(int mLeft,int mTop,int mRight, int mBottom){
            this->marginLeft_ = mLeft;
            this->marginTop_ = mTop;
            this->marginRight_ = mRight;
            this->marginBottom_ = mBottom;
            return static_cast<T&>(*this);
        }

        template<typename T>
        T& setBackgroundConnerRadius(float radius){
            this->bgConnerRadius_ = radius;
            return static_cast<T&>(*this);
        }

        template<typename T>
        T& setLayoutWeight(int weight){
            this->layoutWeight_ = weight;
            return static_cast<T&>(*this);
        }

        template<typename T>
        T& setLayoutGravity(LayoutGravity gravity){
            this->layoutGravity_ = gravity;
            return static_cast<T&>(*this);
        }

        template<typename T>
        T& setId(std::string id){
            this->id = id;
            return static_cast<T&>(*this);
        }

        template<typename T>
        T& setClick(std::function<void(InputEvent&)> clickFn){
            this->onClickFn = clickFn;
            return static_cast<T&>(*this);
        }

    protected:
        Container *parent_ = nullptr;
        
        glm::vec4 bgColor_ = ConverColorValue(Color::Transparent);
        float bgConnerRadius_ = 0.0f;

        int requestWidth_ = LAYOUT_UNSET;
        int requestHeight_ = LAYOUT_UNSET;

        int width_ = 0;
        int height_ = 0;

        int paddingLeft_ = 0;
        int paddingRight_ = 0;
        int paddingTop_ = 0;
        int paddingBottom_ = 0;

        int marginLeft_ = 0;
        int marginTop_ = 0;
        int marginRight_ = 0;
        int marginBottom_ = 0;

        LayoutGravity layoutGravity_ = LayoutGravity::TopLeft;
        
        int layoutWeight_ = 0;

        VisibleState visible_ = Normal;
        
        std::function<void(InputEvent&)> onClickFn = nullptr;

        bool needEatInputBeginEvent();

        void setSelfUiRootTarget();
    private:
        UiRoot *rootUi = nullptr;

        bool actionMoveOutScope = false;
    };//end class
}



