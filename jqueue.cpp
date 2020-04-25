// CMSC 341 - Spring 2020 - Project 4
// JQueue: a job queue using a skew heap and function pointers

#include "jqueue.h"

using std::cout;
using std::endl;
using std::domain_error;

JQueue::JQueue(prifn_t priFn) {
    _heap = nullptr;
    _size = 0;
    priority = priFn;

}

JQueue::~JQueue() {
    clear();
}

JQueue::JQueue(const JQueue& rhs) {
    if (rhs._heap == nullptr) {
        _heap = nullptr;

    }
    else {
        copyHeap(this->_heap, rhs._heap);
        this->_size = rhs._size;
        this->priority = rhs.priority;
    }

}

JQueue& JQueue::operator=(const JQueue& rhs) {
    if (this == &rhs) {
        return *this;
    }
    else {
        clear();
        if (rhs._heap == nullptr) {
            this->_heap = nullptr;
        }
        else {
            copyHeap(this->_heap, rhs._heap);
            this->_size = rhs._size;
            this->priority = rhs.priority;
            
        }
    }
    return *this;
}

void JQueue::insertJob(const job_t& input) {
    if (empty()) {
        _heap = new Node(input);
        _size++;
        return;
    }
    else {
        Node* newNode = new Node(input);
        _heap = merge(newNode, _heap);
        _size++;
    }
}

job_t JQueue::getNextJob() {
    if (empty()) {
        throw domain_error("the queue is empty!");
    }
    else {
        job_t firstJob = _heap->_job;
        Node* temp = _heap;
        _heap = merge(temp->_left, temp->_right);
        _size--;
        delete temp;
        return firstJob;
    }

}

void JQueue::mergeWithQueue(JQueue& rhs) {
    if (this->priority == rhs.priority) {
        this->_heap = merge(this->_heap, rhs._heap);
        rhs._heap = nullptr;
    }
    else {
        throw domain_error("The merged job queues should have the same priority function");
    }

}

void JQueue::clear() {
    makeEmpty(_heap);
    _size = 0;
}

int JQueue::numJobs() const {
    return _size;
}

void JQueue::printJobQueue() const {
    cout << endl;
    preorder(_heap);
}

prifn_t JQueue::getPriorityFn() const {
    return priority;
}

void JQueue::setPriorityFn(prifn_t priFn) {
    if (priority != priFn) {
        priority = priFn;

    }
    

}

void JQueue::dump() const {
    inorder(_heap);
}

void JQueue::makeEmpty(Node* root)
{
    if (root != nullptr) {
        makeEmpty(root->_left);
        makeEmpty(root->_right);
        delete root;
        root = nullptr;
    }
}

void JQueue::copyHeap(Node* copied, Node* otherTree)
{
    if (otherTree == nullptr) {
        copied = nullptr;
    }
    else {
        copied = new Node(otherTree->_job);
        copyHeap(copied->_left, otherTree->_left);
        copyHeap(copied->_right, otherTree->_right);
    }
}

void JQueue::preorder(Node* root) const
{
    if (root == nullptr) {
        return;
    }else{
        cout << "[" << priority(root->_job) << "]" << " ";
        cout  << root->_job << " ";
        cout << endl;
        preorder(root->_left);
        preorder(root->_right);
    }
}

void JQueue::inorder(Node* root) const
{
    if (root == nullptr) {
        return;
    }
    else{
        cout << "(";
        inorder(root->_left);
        cout << priority(root->_job);
        inorder(root->_right);
        cout << ")";
    }
}

Node* JQueue::merge(Node* heap1, Node* heap2)
{
    Node* temp;
    if (heap1 == nullptr) {
        return heap2;
    }
    else if (heap2 == nullptr) {
        return heap1;
    }
    else {
        if (priority(heap2->_job) > priority(heap1->_job)) {
            std::swap(heap1, heap2);
        }

        temp = heap1->_left;
        heap1->_left = heap1->_right;
        heap1->_right = temp;
        heap1->_left = merge(heap2, heap1->_left);
        return heap1;
    }

    //return nullptr;
}

bool JQueue::empty()
{
    if (_heap == nullptr) {
        return true;
}
    else {
        return false;
    }

}

ostream& operator<<(ostream& sout, const job_t& job) {
    sout << "Job: " << job._name << ", pri: " << job._priority
        << ", u: " << job._user << ", g: " << job._group << ", proc: "
        << job._proc << ", mem: " << job._mem << ", time: " << job._time;
    return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
    sout << node._job;
    return sout;
}

