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
    std::vector<int> indicesToGet;
    ARROW_RETURN_NOT_OK(arrow_reader->ReadTable(&table));
    auto recordListCol1 = arrow::Table::Make(arrow::schema({table->schema()->GetFieldByName("recordList")}),
                                             {table->GetColumnByName("recordList")});
    for (int i = 0; i < 20; ++i)
    {
        cout << "data reread operation number = " + std::to_string(i) << endl;
        std::shared_ptr<arrow::Table> table2;
        ARROW_RETURN_NOT_OK(arrow_reader->ReadTable(&table2));
        auto recordListCol2 = arrow::Table::Make(arrow::schema({table2->schema()->GetFieldByName("recordList")}),
                                                 {table2->GetColumnByName("recordList")});
        bool equals = recordListCol1->Equals(*recordListCol2);
        if (!equals)
        {
            cout << recordListCol1->ToString() << endl;
            cout << endl
                 << "new table" << endl;
            cout << recordListCol2->ToString() << endl;
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
