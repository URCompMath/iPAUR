template<typename aType>
MorphologicalFilter<aType>::MorphologicalFilter(Image<aType>& src) {
    this->height = src.Height();
    this->width = src.Width();
    this->channel = src.Channels();
    this->type = src.Type();
    this->filtered = new aType[this->height*this->width*this->channel];
}

template<typename aType>
MorphologicalFilter<aType>::~MorphologicalFilter() {
    delete[] filtered;
}

template<typename aType>
void MorphologicalFilter<aType>::MedianOfArray(aType* kernel, int diameter) {
    int tmp;
    for (int i = diameter - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (kernel[j] > kernel[j + 1]) {
                tmp = kernel[j + 1];
                kernel[j] = kernel[j + 1];
                kernel[j + 1] = tmp;
            }
        }
    }
}

template<typename aType>
void MorphologicalFilter<aType>::CreateOnes(aType* kernel, int diameter) {
    for (int i = 0; i < diameter; i++)
        kernel[i] = 1.0;
}

template<typename aType>
void MorphologicalFilter<aType>::FilterDx(Image<aType>& src, aType* kernel, int radius, char factor) {
    int i, j, k, l, inf;
    for (k = 0; k < channel; k++) {
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                inf = 255;
                for (l = -radius; l <= radius; l++) {
                    if (j + l < 0) {
                        inf = (kernel[l + radius] == 1 && factor * src.Get(i, 0, k) < inf) ? factor * src.Get(i, 0, k) : inf;
                    } else if (j + l >= width) {
                        inf = (kernel[l + radius] == 1 && factor * src.Get(i, width - 1, k) < inf) ? factor * src.Get(i, width - 1, k) : inf;
                    } else {
                        inf = (kernel[l + radius] == 1 && factor * src.Get(i, j + l, k) < inf) ? factor * src.Get(i, j + l, k) : inf;
                    }
                }
                filtered[j + i * width + k * height * width] = factor * inf;
            }
        }
    }
}

template<typename aType>
void MorphologicalFilter<aType>::FilterDy(Image<aType>& dst, aType* kernel, int radius, char factor) {
    int i, j, k, l, inf;
    for (k = 0; k < channel; k++) {
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                inf = filtered[j + i * width + k * height * width];
                for (l = -radius; l <= radius; l++) {
                    if (i + l < 0) {
                        inf = (kernel[l + radius] == 1 && factor * filtered[j + k * height * width] < inf) ? factor * filtered[j + k * height * width] : inf;
                    } else if (i + l >= height) {
                        inf = (kernel[l + radius] == 1 && factor * filtered[j + (height - 1) * width + k * height * width] < inf) ? factor * filtered[j + (height - 1) * width + k * height * width] : inf;
                    } else {
                        inf = (kernel[l + radius] == 1 && factor * filtered[j + (i + l) * width + k * height * width] < inf) ? factor * filtered[j + (i + l) * width + k * height * width] : inf;
                    }
                }
                dst.Set(i, j, k, factor * inf);
            }
        }
    }
}

template<typename aType>
void MorphologicalFilter<aType>::Inverse(Image<aType>& src, Image<aType>& dst) {
    dst.Reset(src.Height(), src.Width(), src.Channels(), src.Type());
    for (int k = 0; k < src.Channels(); k++)
        for (int i = 0; i < src.Height(); i++)
            for (int j = 0; j < src.Width(); j++)
                dst.Set(i, j, k, 255 - src.Get(i, j, k));
}

template<typename aType>
void MorphologicalFilter<aType>::Erosion(Image<aType>& src, Image<aType>& dst, int radius) {
    dst.Reset(src.Height(), src.Width(), src.Channels(), src.Type());
    int diameter = 2 * radius + 1;
    aType* kernel = new aType[diameter];
    CreateOnes(kernel, diameter);
    FilterDx(src, kernel, radius, 1);
    FilterDy(dst, kernel, radius, 1);
    delete[] kernel;
}

template<typename aType>
void MorphologicalFilter<aType>::Dilatation(Image<aType>& src, Image<aType>& dst, int radius) {
    dst.Reset(src.Height(), src.Width(), src.Channels(), src.Type());
    int diameter = 2 * radius + 1;
    aType* kernel = new aType[diameter];
    CreateOnes(kernel, diameter);
    FilterDx(src, kernel, radius, -1);
    FilterDy(dst, kernel, radius, -1);
    delete[] kernel;
}

template<typename aType>
void MorphologicalFilter<aType>::Median(Image<aType>& src, Image<aType>& dst, int radius) {
    dst.Reset(src.Height(), src.Width(), src.Channels(), src.Type());
    int i, j, k, l, m, x, y;
    int diameter = pow(2 * radius + 1, 2);
    aType* kernel = new aType[diameter];
    for (k = 0; k < channel; k++) {
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                for (l = -radius; l <= radius; l++) {
                    for (m = -radius; m <= radius; m++) {
                        x = i + l >= height ? height - 1 : (i + l < 0 ? 0 : i + l);
                        y = j + m >= width ? width - 1 : (j + m < 0 ? 0 : j + m);
                        kernel[(m + radius) + (l + radius) * (2 * radius + 1)] = src.Get(x, y, k);
                    }
                }
                MedianOfArray(kernel, diameter);
                dst.Set(i, j, k, kernel[(diameter - 1) / 2]);
            }
        }
    }
    delete[] kernel;
}

template<typename aType>
void MorphologicalFilter<aType>::Open(Image<aType>& src, Image<aType>& dst, int radius) {
    Dilatation(src, dst, radius);
    Erosion(dst, dst, radius);
}

template<typename aType>
void MorphologicalFilter<aType>::Close(Image<aType>& src, Image<aType>& dst, int radius) {
    Erosion(src, dst, radius);
    Dilatation(dst, dst, radius);
}