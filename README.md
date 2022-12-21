# RANSAC
A sample RANSAC design

输入：
模型model，观测值data
确定模型最少需要多少个数据 n
算法迭代的次数 k
判断内点的阈值 t
判定模型是否适用于数据集的数目 d
输出：
best_model , best fit model parameters , if fail return nullptr

best_consensus_set , inliners

best_error 



iterations = 0;
best_model = null
best_consensus_set = T_MAX;

while(iterations++ < k){
    guess_datas = 随机选择n 个点；

    guess_model = 根据gussdatas生成模型；设计成类方法
    
    guess_consensus_set = gusstatas 设计成类方法

    for(除了guess_datas以外的点){
        if(？？如果点适合guess_model？？ , 误差小于t){
            加入到guess_consensus_set;
        }
    }

    if(guess_consensus_set的数量大于d){
        已经找到了模型，测试现在的模型有多好
        better_model = 适合于guess_consensus_set中所有点的模型；
        temp_error = better_model的误差
        if(temp_error<best_error){
            best_model = better_model;
            best_consensus_set = guess_consensus_set;
            best_error = temp_eror
        }
    }
}
