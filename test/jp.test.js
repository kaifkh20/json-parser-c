import {expect,test,beforeAll,afterAll} from "bun:test"

beforeAll(async()=>{
    const proc =  Bun.spawn(["make"])
    await proc.exited
})

async function runScript(file_path){
    const proc = Bun.spawn(["./jp","-f",`${file_path}`])
    const output = (await new Response(proc.stdout).text())

    await proc.exited

    return output
}

test("Check for Valid Empty JSON",async()=>{
    // const json_string = "{}"
    const output = await runScript("test/tests_file/test1.json")
    expect(output).toEqual("Succesfully Parsed\n")
})

test("Checking for single key value pair",async()=>{
    const output = await runScript("test/tests_file/test2.json")
    expect(output).toEqual("value\nSuccesfully Parsed\n")
})

afterAll(()=>{
    const proc = Bun.spawn(['make','clean'])
    proc.exited
})

