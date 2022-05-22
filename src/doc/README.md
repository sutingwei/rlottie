
软光栅化渲染集中三个函数：update / render (preprocessStage & render)
   
a. update更新图层的当前帧状态信息，获取最新的矩阵信息等 

b. preprocessStage通过a步骤获取的信息，生成FT需要的数据结构，交由FT，生成图层的光栅化数据

c. 混合b步骤生成的图层光栅化数据

考虑基于canvas渲染，preprocessStage和混合的两个步骤需要整合到一起（图形API为立即模式，考虑FrameBuffer?）

mRenderNode承载渲染能力，统一对它设置渲染属性