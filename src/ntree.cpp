#include "ntree.hpp"

template class NTree<int>;

template <typename T>
NTree<T>::NTree(size_t max_child, T data, std::string index)
    : max_child{max_child}
    , data{data}
    , index{index} 
    {}  

template <typename T>        
void NTree<T>::print(int level) { 
    std::string indent(level * 4, ' ');   
    std::cout << indent << data << " (Index: " << (index  == "" ? "Root" : index) << ")" << std::endl; 
    for (auto& child : children) {        
        child->print(level + 1); 
    }
}

template <typename T>        
void NTree<T>::addChild(T data, const std::string& index)  {  
    if(children.size() < max_child) {
        children.push_back(std::make_unique<NTree<T>>(max_child, data, index));   
    }
}

template <typename T>
void NTree<T>::addChild(T data, size_t child_index) {
    if (child_index < children.size()) {
        children[child_index]->addChild(data, children[child_index]->index + std::to_string(child_index));
    } else {
        std::cerr << "Child index out of range!" << std::endl;
    }
}

template <typename T>        
NTree<T>* NTree<T>::getChild(size_t child_index) {       
    NTree* result = nullptr;     
    if(child_index < children.size()) {       
        result = children[child_index].get();  
    }
    return result;   
}

template <typename T>
T NTree<T>::getChildValue(size_t child_index) {
    T result = T();
    if(child_index < children.size()) {
        result = children[child_index]->data;     
    }
    return result;
}

template <typename T>
void NTree<T>::generateChildren(int depth, float probability) {
    int current_max_children = max_child;
    for(int i = 0; i < current_max_children && depth > 0;){
        if((float)rand()/RAND_MAX <= probability) {
            std::string currIndex = index+std::to_string(i++);
            addChild(depth, currIndex);

            children.back()->generateChildren(depth-1, probability);   
        } else {
            --current_max_children;
        }
    }
}

template <typename T>
NTree<T>* NTree<T>::findNode(const std::string& index){
    NTree<T>* elem = this;
    for(const char& ch : index) {
        int childIndex = ch - '0';
        if (childIndex < 0 || childIndex >= static_cast<int>(elem->children.size())) {
            return nullptr;
        }
        elem = elem->children[childIndex].get();
    }
    return elem;
}

template <typename T>
std::string NTree<T>::findCommonNode(const std::string& index_one, const std::string& index_two) {
    int min_length = std::min(index_one.size(), index_two.size());
    if(index_one == index_two) min_length--;
    std::string result;
    for(int i = 0; i < min_length && index_one[i] == index_two[i]; i++) {
        result += index_one[i];
    }
    return result;
}

template <typename T>
void NTree<T>::collectNodes(std::vector<NTree<T>*>& nodes){
    nodes.push_back(const_cast<NTree<T>*>(this));
    for(const auto& child : children) {
        child->collectNodes(nodes);
    }
}

template<typename T>
std::pair<NTree<T>*, NTree<T>*>  NTree<T>::getTwoRandomNodes(){

    std::pair<NTree<T>*, NTree<T>*> result = {nullptr, nullptr};
    std::vector<NTree<T>*> nodes;
    
    collectNodes(nodes);
    
    if(nodes.size() > 1) {
        int rand_index_one = rand() % (nodes.size() - 1);
        int rand_index_two = rand() % (nodes.size() - 1);
        result = {nodes[rand_index_one], nodes[rand_index_two]};
    }
    return result;
}

template<typename T>
void NTree<T>::getTwoRandomAndFindCommon() {
    auto [ind1, ind2] = getTwoRandomNodes();

    if(ind1 != nullptr && ind2 != nullptr) {
        std::cout << "First random node index: " << ind1->index << std::endl;
        std::cout << "Second random node index: " << ind2->index << std::endl;
        
        std::string parent = findCommonNode(ind1->index, ind2->index);
        if(parent == "") {
            parent += "Root";
        }
        std::cout << "Their parent index: " << parent << std::endl;
    } else {
        std::cout << "Not enough nodes to choose!" << std::endl;
    }
}

