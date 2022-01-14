#include<memory>
#include<iostream>
#include<stdexcept>
#include<filesystem>
#include<arrow/api.h>
#include<arrow/io/api.h>
#include<parquet/arrow/reader.h>

using namespace std;
using namespace std::__fs;

arrow::Status inner_main() {
    filesystem::path filePath = "writeReadRowGroup.parquet";
    arrow::MemoryPool *pool = arrow::default_memory_pool();
    std::shared_ptr<arrow::io::ReadableFile> infile;
    PARQUET_ASSIGN_OR_THROW(infile, arrow::io::ReadableFile::Open(filePath, pool));
    std::unique_ptr<parquet::arrow::FileReader> arrow_reader;
    auto status = parquet::arrow::OpenFile(infile, pool, &arrow_reader);
    ARROW_RETURN_NOT_OK(status);
    std::shared_ptr<arrow::Schema> readSchema;
    ARROW_RETURN_NOT_OK(arrow_reader->GetSchema(&readSchema));
    std::shared_ptr<arrow::Table> table;

    ARROW_RETURN_NOT_OK(arrow_reader->ReadTable(&table));
    cout << table->GetColumnByName("recordList")->ToString() << endl;

    for (int i = 0; i < 20; ++i)
    {
        cout << "data reread operation number = " + std::to_string(i) << endl;
        std::shared_ptr<arrow::Table> table2;
        ARROW_RETURN_NOT_OK(arrow_reader->ReadTable(&table2));
        bool equals = table->GetColumnByName("recordList")->Equals(table2->GetColumnByName("recordList"));
        if (!equals)
        {
            cout << endl
                 << "new table" << endl;
            cout << table2->GetColumnByName("recordList")->ToString() << endl;
            throw std::runtime_error("Subsequent re-read failure ");
        }
    }

    return arrow::Status::OK();
}

int main()
{
    auto status = inner_main();
    return status.ok() ? 0 : 1;
}
