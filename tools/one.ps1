param(
    $name='test/file.md'
)
cls

switch -Wildcard ($name) {
    '*.md' {
        xmake
        ./build/windows/x64/release/mdCompiler.exe $name
    }
    '*.txt' {
        Write-Host "Processing a text file: $name"
        # Add your code here for processing text files
    }
    default {
        Write-Host "Unsupported file type: $name"
    }
}



