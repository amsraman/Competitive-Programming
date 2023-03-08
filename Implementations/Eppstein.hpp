template <typename T>
vector<T> eppstein(vector<vector<tuple<int, T, int>>> & graph, vector<vector<tuple<int, T, int>>> & reverse_graph, int s, int t, int k) {
    using LeftistHeap = Node<pair<T, int>, true>*;
    using state = tuple<T, int, LeftistHeap>;
    int n = graph.size();
    T INF = numeric_limits<T>::max();
    vector<int> ord;
    vector<pair<int, int>> pv(n, {-1, -1});
    vector<T> dist(n, INF), ans;
    vector<LeftistHeap> heap(n, NULL);
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> q1;
    q1.push({0, t}), dist[t] = 0;
    while(!q1.empty()) {
        pair<T, int> tp = q1.top(); q1.pop();
        if(dist[tp.s] < tp.f) {
            continue;
        }
        ord.push_back(tp.s);
        for(tuple<int, T, int> edge: reverse_graph[tp.s]) {
            if(dist[tp.s] + get<1>(edge) < dist[get<0>(edge)]) {
                dist[get<0>(edge)] = dist[tp.s] + get<1>(edge), pv[get<0>(edge)] = {get<2>(edge), tp.s};
                q1.push({dist[get<0>(edge)], get<0>(edge)});
            }
        }
    }
    for(int v: ord) {
        for(tuple<int, T, int> edge: graph[v]) {
            if(get<2>(edge) != pv[v].f && dist[get<0>(edge)] < INF) {
                heap[v] = insert(heap[v], {dist[get<0>(edge)] + get<1>(edge) - dist[v], get<0>(edge)});
            }
        }
        if(v != t) {
            heap[v] = meld(heap[v], heap[pv[v].s]);
        }
    }
    priority_queue<state, vector<state>, greater<state>> q2;
    if(dist[s] != INF) {
        ans.push_back(dist[s]);
        if(heap[s]) {
            q2.push({heap[s]->val.f, s, heap[s]});
        }
    }
    while(!q2.empty() && ans.size() < k) {
        auto tp = q2.top(); q2.pop();
        T cost = get<0>(tp); int cur = get<1>(tp); LeftistHeap cur_heap = get<2>(tp);
        ans.push_back(cost + dist[s]);
        if(heap[cur_heap->val.s]) {
            q2.push({cost + heap[cur_heap->val.s]->val.f, cur_heap->val.s, heap[cur_heap->val.s]});
        }
        if(cur_heap->left) {
            q2.push({cost + cur_heap->left->val.f - cur_heap->val.f, cur, cur_heap->left});
        }
        if(cur_heap->right) {
            q2.push({cost + cur_heap->right->val.f - cur_heap->val.f, cur, cur_heap->right});
        }
    }
    ans.resize(k, -1);
    return ans;
}