#pragma once

namespace Ocean {

    namespace Vulkan {

        /**
         * @brief The DataStream is the class that handles streaming submitted data (Vertices / Indices) to the GPU.
         * 
         */
        class DataStream {
        public:
            DataStream() { }
            ~DataStream() = default;

            void AddData(void* data);
            void ClearData();

        private:


        };

    }   // Vulkan

}   // Ocean