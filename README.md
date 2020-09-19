# 3DCity
点光源和方向光修复
在面绘制的时候需要判断顺逆时针  图形绘制的顺序判断 利用格林公式
revise the normal vector and so the light can be reflect correctly
现存在问题为 地面的shadow mapping 能绘制到地面的ground 上但是与上面的重复绘制的图形不重合
可能存在的问题可能为ground 和 screen 坐标的转换或存在比例不一致导致，还有quads as ground时的坐标和screen 的坐标可能没有对应
glviewport 会影响scene and FBO的大小不能过大因为使用QOpenGLFramebufferObject绘制的时候只能在paintGL（）中进行不像core可以在initial里完成
......
