#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>

using namespace std;

//This describes the photo completely
struct Photo {
    int id;
    char orientation;
    int numOfTags;
    vector <string> tags;
};

struct Slide {
    vector<int> slideId;
    vector<string> slideTags;
};

bool sortByTags(const Photo &a, const Photo &b){
    return a.tags[0] < b.tags[0];
}

bool sortBySlideTags(const Slide &a, const Slide &b){
    return a.slideTags[0] < b.slideTags[0];
}

//Sort Slideshow by tags in slide
//bool sortByTags(const Slide &a, const Slide &b){
//    return includes(a.slideTags.begin(), a.slideTags.end(), b.slideTags.begin(), b.slideTags.end());
//}

int main(){
    ifstream in_file;
    in_file.open("../e_shiny_selfies.txt");

    if(!in_file){
        cerr << "Error opening file" << endl;
        return 1;
    }

    int photoNum;

    in_file >>  photoNum;

    //Declare collection of photo
    vector <Photo> photoCollection;

    for (int i = 0; i < photoNum; ++i) {
        Photo photo;
        photo.id = i;
        in_file >> photo.orientation >> photo.numOfTags; //Receive the orientation and num of tags

        for (int j = 0; j < photo.numOfTags; ++j) { //Receive all the tags given the num of tags
            string tag;
            in_file >> tag;
            photo.tags.push_back(tag); //Push each tag to the array of tags
        }

        //Push the photo to the collection of photos
        sort(photo.tags.begin(), photo.tags.end());
        photoCollection.push_back(photo);
    } //Photo Collection successfully created

    in_file.close();

    sort(photoCollection.begin(), photoCollection.end(), sortByTags);

    //Next is to create the slideshow
    vector<Slide> slideShow;
    //vector<Photo> slide;
    //Looping through the collection
    //If orientation is H, it is a slide, and two vertical makes a slide
    for (int k = 0; k < photoNum; ++k) {
        Slide slide;
        if (photoCollection[k].orientation == 'H'){
            slide.slideId.push_back(photoCollection[k].id);
            for (int i = 0; i < photoCollection[k].numOfTags; ++i) {
                slide.slideTags.push_back(photoCollection[k].tags[i]);
            }
            slideShow.push_back(slide);
        }
    }

    //Push vertical slides
    Slide slide;
    for (int i = 0; i < photoNum; ++i) {
        if (photoCollection.at(i).orientation == 'V'){
            slide.slideId.push_back(photoCollection[i].id);
            for (int j = 0; j < photoCollection[i].numOfTags; ++j) {
                slide.slideTags.push_back(photoCollection[i].tags[j]);
            }
        }
        if (slide.slideId.size() == 2){
            slide.slideTags.erase(unique( slide.slideTags.begin(),  slide.slideTags.end()),  slide.slideTags.end());
            slideShow.push_back(slide);
            slide.slideTags.clear();
            slide.slideId.clear();
        }

    }

    sort(slideShow.begin(), slideShow.end(), sortBySlideTags);

    //Write to output
    fstream out_file;
    out_file.open("../output.txt", ios::out);

    if (!out_file) {
        cerr << "Unable to create file" << endl;
        return 1;
    }

    out_file << slideShow.size() << endl;
    for (auto &slide : slideShow) {
        for (int i = 0; i < slide.slideId.size(); ++i) {
            out_file << slide.slideId[i]<< " ";
        }
        out_file << endl;
    }

    return 0;
}